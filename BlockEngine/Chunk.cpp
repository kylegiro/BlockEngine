#include "Chunk.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Chunk::Chunk(Texture& texture) : texture(texture)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 

 	blocks = new Block**[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		blocks[i] = new Block*[SIZE];
		for (int j = 0; j < SIZE; j++)
		{
			blocks[i][j] = new Block[SIZE];
		}
	}

    rebuildMesh();
}

Chunk::~Chunk()
{
	for (int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			delete[] blocks[i][j];
		}
		delete[] blocks[i];
	}
	delete[] blocks;
}

void Chunk::render(Shader& shader)
{
    // bind texture
    glUniform1i(shader.getUniformLocation("tex"), 0);
    texture.bind();

    // transformations
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::rebuildMesh()
{
    vertices.clear();
    indices.clear();

	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			for (int z = 0; z < SIZE; z++)
			{
				if (blocks[x][y][z].getType() == Block::Type::AIR)
					continue;

                addBlockToMesh(x, y, z);
			}			           
		}
	}

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "vertices: " << this->vertices.size() << std::endl << "indices: " << this->indices.size() << std::endl;
}

void Chunk::addBlockToMesh(int x, int y, int z)
{

    std::vector<float> vertices = {
        // front face
        1.0f + x, -1.0f + y, -1.0f + z,     0.0f, 0.0f,
        -1.0f + x, -1.0f + y, -1.0f + z,    1.0f, 0.0f,
        -1.0f + x, 1.0f + y, -1.0f + z,     1.0f, 1.0f,
        1.0f + x, 1.0f + y, -1.0f + z,      0.0f, 1.0f,
        // back face
        -1.0f + x, -1.0f + y, 1.0f + z,     0.0f, 0.0f,
        1.0f + x, -1.0f + y, 1.0f + z,      1.0f, 0.0f,
        1.0f + x, 1.0f + y, 1.0f + z,       1.0f, 1.0f,
        -1.0f + x, 1.0f + y, 1.0f + z,      0.0f, 1.0f,
        // right face
       1.0f + x, -1.0f + y, 1.0f + z,      0.0f, 0.0f,
       1.0f + x, -1.0f + y, -1.0f + z,     1.0f, 0.0f,
       1.0f + x, 1.0f + y, -1.0f + z,      1.0f, 1.0f,
       1.0f + x, 1.0f + y, 1.0f + z,       0.0f, 1.0f,
       // left face
      -1.0f + x, -1.0f + y, -1.0f + z,    0.0f, 0.0f,
      -1.0f + x, -1.0f + y, 1.0f + z,     1.0f, 0.0f,
      -1.0f + x, 1.0f + y, 1.0f + z,      1.0f, 1.0f,
      -1.0f + x, 1.0f + y, -1.0f + z,     0.0f, 1.0f,
      // bottom face
      -1.0f + x, -1.0f + y, -1.0f + z,    0.0f, 1.0f,
      1.0f + x, -1.0f + y, -1.0f + z,     1.0f, 1.0f,
      1.0f + x, -1.0f + y, 1.0f + z,      1.0f, 0.0f,
      -1.0f + x, -1.0f + y, 1.0f + z,     0.0f, 0.0f,
      // top face
      1.0f + x, 1.0f + y, -1.0f + z,      1.0f, 1.0f,
      -1.0f + x, 1.0f + y, -1.0f + z,     0.0f, 1.0f,
      -1.0f + x, 1.0f + y, 1.0f + z,      0.0f, 0.0f,
      1.0f + x, 1.0f + y, 1.0f + z,       1.0f, 0.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,        // front low tri
        0, 2, 3,        // front high tri        
        4, 5, 6,        // back low tri
        4, 6, 7,        // back high tri
        8, 9, 10,      // right low tri
        8, 10, 11,      // right high tri
        12, 13, 14,     // left low tri
        12, 14, 15,     // left high tri
        16, 18, 19,     // bottom low tri
        16, 17, 18,     // bottom high tri
        20, 23, 22,     // top low tri
        20, 21, 22      // top high tri
    };

    for (int v = 0; v < vertices.size(); v++)
    {
        this->vertices.push_back(vertices[v]);
    }

    for (int i = 0; i < indices.size(); i++)
    {
        unsigned int index = 24 * nextIndex + indices[i];        
        this->indices.push_back(index);
    }

    nextIndex++;
}
