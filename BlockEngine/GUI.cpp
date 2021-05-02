#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <sstream>

#include "Util.h"

#include "Engine.h"

// #define DEBUG_NEIGHBORS
#define DEBUG_HEIGHTMAP
//#define DEBUG_REBUILDCHUNK


GUI::GUI(SDL_Window* window, SDL_GLContext glContext, ChunkManager& chunkManager, NoiseMap& heightMap, Engine& engine)
	: window(window), glContext(glContext), chunkManager(chunkManager), heightMap(heightMap), engine(engine)
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

#ifdef DEBUG_HEIGHTMAP	
	std::ostringstream hmss;
	double x = floor(camera.getPosition().x);
	double z = floor(camera.getPosition().z);
	double height = heightMap.getValue(x, z);
	
	hmss << "HeightMap: " << height;

	ImGui::Text(hmss.str().c_str());
#endif

	ImGui::Checkbox("Render Debug", engine.getDebugModeAdr());


#ifdef DEBUG_NEIGHBORS	
	std::ostringstream ss;

	glm::ivec3 pos = worldToChunk(camera.getPosition());
	Chunk* chunk = chunkManager.getChunk(pos.x, pos.y, pos.z);
	if (chunk != nullptr)
	{
		Chunk* xNeg = chunk->getXNeg();
		if (xNeg != nullptr)
		{
			ss << "xNeg: (" << xNeg->getX() << "," << xNeg->getY() << "," << xNeg->getZ() << ")";
			ImGui::Text(ss.str().c_str());
			ss.str(std::string());
		}

		Chunk* xPos = chunk->getXPos();
		if (xPos != nullptr)
		{
			ss << "xPos: (" << xPos->getX() << "," << xPos->getY() << "," << xPos->getZ() << ")";
			ImGui::Text(ss.str().c_str());
			ss.str(std::string());
		}

		Chunk* yNeg = chunk->getYNeg();
		if (yNeg != nullptr)
		{
			ss << "yNeg: (" << yNeg->getX() << "," << yNeg->getY() << "," << yNeg->getZ() << ")";
			ImGui::Text(ss.str().c_str());
			ss.str(std::string());
		}

		Chunk* yPos = chunk->getYPos();
		if (yPos != nullptr)
		{
			ss << "yPos: (" << yPos->getX() << "," << yPos->getY() << "," << yPos->getZ() << ")";
			ImGui::Text(ss.str().c_str());
			ss.str(std::string());
		}


		Chunk* zNeg = chunk->getZNeg();
		if (zNeg != nullptr)
		{
			ss << "zNeg : (" << zNeg->getX() << "," << zNeg->getY() << "," << zNeg->getZ() << ")";
			ImGui::Text(ss.str().c_str());
			ss.str(std::string());
		}

		Chunk* zPos = chunk->getZPos();
		if (zPos != nullptr)
		{
			ss << "zPos: (" << zPos->getX() << "," << zPos->getY() << "," << zPos->getZ() << ")";
			ImGui::Text(ss.str().c_str());
			ss.str(std::string());
		}

		ss << "Vertices: " << chunk->getNumVertices();
		ImGui::Text(ss.str().c_str());
		ss.str(std::string());

		ss << "Indices: " << chunk->getNumIndices();
		ImGui::Text(ss.str().c_str());
		ss.str(std::string());
	}
#endif

#ifdef DEBUG_REBUILDCHUNK
	if (ImGui::Button("Rebuild Chunk"))
	{
		glm::ivec3 pos = worldToChunk(camera.getPosition());
		Chunk* chunk = chunkManager.getChunk(pos.x, pos.y, pos.z);
		if (chunk != nullptr)
			chunkManager.updateNeighbors(chunk, pos.x, pos.y, pos.z);
			chunk->setNeedsRebuild(false, true);
			chunk->rebuildMesh(chunkManager);			
	}
#endif
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
	

	glm::ivec3 chunkCoord = worldToChunk(position);
	ss << "Chunk: (" << chunkCoord.x << "," << chunkCoord.y << "," << chunkCoord.z << ")";
	chunkPos = ss.str();
	ss.str(std::string());
}
