#pragma once

#include <vector>
#include <unordered_map>

#include "Chunk.h"
#include "Shader.h"

typedef std::vector<Chunk*> ChunkList;

const unsigned int MAX_CHUNKS = 32;

struct ChunkCoord {
	int x;
	int y;
	int z;

	bool operator==(const ChunkCoord& other) const {
		return x == other.x && y == other.y && z == other.z;
	};

	struct Hash
	{
		size_t operator()(const ChunkCoord& chunk) const
		{
			size_t xHash = std::hash<int>()(chunk.x);
			size_t yHash = std::hash<int>()(chunk.y) << 1;
			size_t zHash = std::hash<int>()(chunk.z) << 2;
			return xHash ^ yHash ^ zHash;
		}
	};
};

class ChunkManager
{
public:
	ChunkManager(Shader& shader, Texture& texture);
	void update(double dt);
	void render();
	void updateLoadList();
	void updateSetupList();
	void updateRebuildList();
	void updateFlagsList();
	void updateUnloadList();
	void updateVisibilityList();
	void updateRenderList();
	Chunk* getChunk(int x, int y, int z);	

private:
	std::unordered_map<ChunkCoord, Chunk*, ChunkCoord::Hash> chunks;
	ChunkList loadList, setupList, rebuildList, flagsList, unloadList, visibilityList, renderList;
	bool forceVisibilityUpdate = false;

	Shader& shader;
	Texture& texture;
};

