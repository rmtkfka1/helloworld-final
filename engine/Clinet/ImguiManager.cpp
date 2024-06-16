#include "pch.h"
#include "ImguiManager.h"
#include "Core.h"
#include "LightManager.h"

ImguiManager::~ImguiManager()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::Init()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(core->GetWindowInfo().hwnd);
	ImGui_ImplDX12_Init(core->GetDevice().Get(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM, core->GetImguiHeap().Get(),
		core->GetImguiHeap()->GetCPUDescriptorHandleForHeapStart(),
		core->GetImguiHeap()->GetGPUDescriptorHandleForHeapStart());

}

void ImguiManager::Render()
{


	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("2022180005");                          // Create a window called "Hello, world!" and append into it.

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::Text("x: = %f,y: = %f,z: = %f", _temp.x, _temp.y ,_temp.z);
		ImGui::Text("x: = %f,y: = %f,z: = %f", _temp2.x, _temp2.y, _temp2.z);
		
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}


	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), core->GetCmdList().Get());


}
