#pragma once
#include <Windows.h>
#include <xorstr.hpp>
#include <dwmapi.h> 
#include <imgui.h>
#include <dxgi.h>
#include <d3d11.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#pragma comment(lib,"d3d11.lib")

class Nvidia
{
public:

	auto window_set_style()-> void;

	auto window_set_transparency()-> void;

	auto window_set_top_most()-> void;

	auto retrieve_window()->HWND;

	auto window_init()->BOOL;

	auto d3d11_init()->BOOL;

	auto d3d11_shutdown()->void;

	auto imgui_init() -> void;

	auto begin_scene() -> void;

	auto end_scene()-> void;

	auto clear_scene()->void;

	auto imgui_shutdown()->void;

private:
	HWND win;

	ID3D11Device* g_pd3dDevice = NULL;

	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;

	IDXGISwapChain* g_pSwapChain = NULL;

	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
};