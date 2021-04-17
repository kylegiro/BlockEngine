#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <sstream>

#include "Util.h"

GUI::GUI(SDL_Window* window, SDL_GLContext glContext, ChunkManager& chunkManager) 
	: window(window), glContext(glContext), chunkManager(chunkManager)
{
	ImGui::CreateContext();
	
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
	ImGui::DestroyContext();
}


void GUI::render(Camera& camera)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	
	ImGui::Begin("Debug");
	//ImVec2 size = ImVec2(200, 100);
	//ImGui::SetWindowSize(size);
	ImGui::Text(cameraPos.c_str());	
	ImGui::Text(chunkPos.c_str());
	if (ImGui::Button("Rebuild Chunk"))
	{
		glm::ivec3 pos = worldToChunk(camera.getPosition());
		Chunk* chunk = chunkManager.getChunk(pos.x, pos.y, pos.z);
		if (chunk != nullptr)
			chunk->setNeedsRebuild(true, true);
	}
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::updateCameraPositionLabel(Camera& camera)
{
	glm::vec3 position = camera.getPosition();

	position.x = floor(position.x * 100) / 100;
	position.y = floor(position.y * 100) / 100;
	position.z = floor(position.z * 100) / 100;

	std::ostringstream ss;
	ss << "Position: (" << position.x << "," << position.y << "," << position.z << ")";
	cameraPos = ss.str();
	ss.str(std::string());
	

	glm::ivec3 chunk = worldToChunk(position);
	ss << "Chunk: (" << chunk.x << "," << chunk.y << "," << chunk.z << ")";
	
	chunkPos = ss.str();
}
