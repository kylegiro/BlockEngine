#pragma once

#include <vector>
#include <unordered_map>

#include "Chunk.h"
#include "Shader.h"
#include "Camera.h"
#include "NoiseMap.h"

const unsigned int LOAD_RADIUS = CHUNK_SIZE * 8;
const unsigned int UNLOAD_RADIUS = LOAD_RADIUS + CHUNK_SIZE * 1;
const unsigned int MAX_CHUNKLOAD_PER_FRAME = 2;

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
			size_t yHash = std::hash<int>()(chunk.y);
			size_t zHash = std::hash<int>()(chunk.z);
			return xHash ^ (yHash << 1) ^ (zHash << 2);
		}
	};
};

typedef std::vector<Chunk*> ChunkList;
typedef std::vector<ChunkCoord> ChunkCoordList;
typedef std::unordered_map<ChunkCoord, Chunk*, ChunkCoord::Hash> ChunkMap;

class ChunkManager
{
public:
	ChunkManager(Shader& shader, Texture& texture, Camera& camera, NoiseMap& heightMap);
	void update(double dt);
	void render();
	void renderDebug();
	Chunk* getChunk(int x, int y, int z);
	void loadChunk(int x, int y, int z);
	void unloadChunk(Chunk* chunk);
	void updateNeighbors(Chunk* chunk, int x, int y, int z);	

private:
	ChunkMap chunks;

	Shader& shader;
	Texture& texture;
	Camera& camera;
	
	NoiseMap& heightMap;
};

