#include "Overlay.hpp"

Nvidia Overlay;



int main()
{
	Overlay.window_init();
	Overlay.d3d11_init();
	Overlay.imgui_init();

	while (true)
	{
		Overlay.begin_scene();
		Overlay.clear_scene();

		[](){	
				ImGuiIO& io = ImGui::GetIO();
				POINT mousePosition;
				GetCursorPos(&mousePosition);
				io.MousePos.x = (float)mousePosition.x;
				io.MousePos.y = (float)mousePosition.y;
				if (GetAsyncKeyState(VK_LBUTTON)) io.MouseDown[0] = true;
				else io.MouseDown[0] = false;		
		}();

		ImGui::Begin("Hello");

		ImGui::Text("Hello from another window!");

		ImGui::Button("Button");

		ImGui::End();

		Overlay.end_scene();
	}
	Overlay.imgui_shutdown();
	Overlay.d3d11_shutdown();

	return 0;
}