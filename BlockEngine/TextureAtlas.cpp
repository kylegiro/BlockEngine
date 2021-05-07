#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(unsigned int width, unsigned int height, std::string path)
	: width(width), height(height), texture(path)
{	
	map["stone"] = AtlasTexture{ 0, 0, 16, 16 };
	map["dirt"] = AtlasTexture{ 16, 0, 16, 16 };
	map["grass_side"] = AtlasTexture{ 32, 0, 16, 16 };
	map["grass_top"] = AtlasTexture{ 48, 0, 16, 16 };
}

UV TextureAtlas::getUV(std::string key)
{
	std::unordered_map<std::string, AtlasTexture>::iterator it = map.find(key);
	if (it != map.end())
	{
		return toUV(map[key]);
	}
	return UV{ glm::vec2(), glm::vec2(), glm::vec2(), glm::vec2() };
}

void TextureAtlas::bind()
{
	texture.bind();
}

UV TextureAtlas::toUV(AtlasTexture& tex)
{	
	// atlas origin is top left, uv origin is bottom left
	
	/*
	UV uv;
	uv.bottomLeft = glm::vec2(0.0, 0.9375);
	uv.bottomRight = glm::vec2(0.0625, 0.9375);
	uv.topLeft = glm::vec2(0.0, 1.0);
	uv.topRight = glm::vec2(0.0625, 1.0);
	*/
	
	UV uv;
	uv.bottomLeft = glm::vec2((float) tex.x / width, (float) (height - tex.y - tex.height) / height);
	uv.bottomRight = glm::vec2((float) (tex.x + tex.width) / width, (float) (height - tex.y - tex.height) / height);
	uv.topLeft = glm::vec2((float) tex.x / width, (float) (height - tex.y) / height);
	uv.topRight = glm::vec2((float) (tex.x + tex.width) / width, (float) (height - tex.y) / height);

	return uv;
}
