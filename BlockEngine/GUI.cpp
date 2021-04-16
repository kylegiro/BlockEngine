#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>

GUI::GUI(SDL_Window* window, SDL_GLContext glContext) : window(window), glContext(glContext)
{
	ImGui::CreateContext();
	
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
	ImGui::DestroyContext();
}


void GUI::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	
	ImGui::Begin("Console");
	ImVec2 size = ImVec2(200, 100);
	ImGui::SetWindowSize(size);
	ImGui::Text("Hello world!");	
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
