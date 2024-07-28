#include "Overlay.hpp"
#include <mutex>
auto Nvidia::window_init() -> BOOL {
    win = FindWindowW(xorstr_(L"CEF-OSC-WIDGET"), xorstr_(L"NVIDIA GeForce Overlay"));
    if (!win)
        return FALSE;

    window_set_style();
    window_set_transparency();
    window_set_top_most();

    return TRUE;
}

auto Nvidia::window_set_style() -> void {
    int i = 0;

    i = (int)GetWindowLong(win, -20);

    SetWindowLongPtr(win, -20, (LONG_PTR)(i | 0x20));

}

auto Nvidia::window_set_transparency() -> void {
    MARGINS margin;
    UINT opacity_flag, color_key_flag, color_key = 0;
    UINT opacity = 0;

    margin.cyBottomHeight = -1;
    margin.cxLeftWidth = -1;
    margin.cxRightWidth = -1;
    margin.cyTopHeight = -1;

    DwmExtendFrameIntoClientArea(win, &margin);

    opacity_flag = 0x02;
    color_key_flag = 0x01;
    color_key = 0x000000;
    opacity = 0xFF;

    SetLayeredWindowAttributes(win, color_key, opacity, opacity_flag);
}

auto Nvidia::window_set_top_most() -> void {
    SetWindowPos(win, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);
    ShowWindow(win, SW_SHOW);
}

auto Nvidia::retrieve_window() -> HWND { return win; }

auto Nvidia::d3d11_init() -> BOOL
{

    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = win;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();

    return true;

}

auto Nvidia::d3d11_shutdown()->void
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

auto Nvidia::imgui_init() -> void
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(win);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

auto Nvidia::begin_scene()->void
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

auto Nvidia::end_scene()->void
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(1, 0); // Present with vsync
}

auto Nvidia::clear_scene() ->void
{
    static const float clear_color_with_alpha[4] = { 0,0,0,0 };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
}

auto Nvidia::imgui_shutdown() ->void
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
