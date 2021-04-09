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

                FaceRenderFlags faces = {true, true, true, true, true, true};
                if (x > 0)
                    faces.xNeg = !blocks[x-1][y][z].isOpaque();
                if (x < SIZE-1)
                    faces.xPos = !blocks[x+1][y][z].isOpaque();
                if (y > 0)
                    faces.yNeg = !blocks[x][y-1][z].isOpaque();
                if (y < SIZE-1)
                    faces.yPos = !blocks[x][y+1][z].isOpaque();
                if (z > 0)
                    faces.zNeg = !blocks[x][y][z-1].isOpaque();
                if (z < SIZE-1)
                    faces.zPos = !blocks[x][y][z+1].isOpaque();

                addBlockToMesh(x, y, z, faces);
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

void Chunk::addBlockToMesh(int x, int y, int z, FaceRenderFlags faces)
{
    if (faces.xNeg)
    {
        float left[] = {
            -1.0f + x, -1.0f + y, -1.0f + z,    0.0f, 0.0f,
            -1.0f + x, -1.0f + y, 1.0f + z,     1.0f, 0.0f,
            -1.0f + x, 1.0f + y, 1.0f + z,      1.0f, 1.0f,
            -1.0f + x, 1.0f + y, -1.0f + z,     0.0f, 1.0f
        };

        for (int v = 0; v < 20; v++)
        {
            this->vertices.push_back(left[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.xPos)
    {
        float right[] = {
           1.0f + x, -1.0f + y, 1.0f + z,      0.0f, 0.0f,
           1.0f + x, -1.0f + y, -1.0f + z,     1.0f, 0.0f,
           1.0f + x, 1.0f + y, -1.0f + z,      1.0f, 1.0f,
           1.0f + x, 1.0f + y, 1.0f + z,       0.0f, 1.0f
        };

        for (int v = 0; v < 20; v++)
        {
            this->vertices.push_back(right[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.yNeg)
    {
        float bottom[] = {
            -1.0f + x, -1.0f + y, -1.0f + z,    0.0f, 1.0f,
            1.0f + x, -1.0f + y, -1.0f + z,     1.0f, 1.0f,
            1.0f + x, -1.0f + y, 1.0f + z,      1.0f, 0.0f,
            -1.0f + x, -1.0f + y, 1.0f + z,     0.0f, 0.0f
        };

        for (int v = 0; v < 20; v++)
        {
            this->vertices.push_back(bottom[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.yPos)
    {
        float top[] = {
            1.0f + x, 1.0f + y, -1.0f + z,      1.0f, 1.0f,
            -1.0f + x, 1.0f + y, -1.0f + z,     0.0f, 1.0f,
            -1.0f + x, 1.0f + y, 1.0f + z,      0.0f, 0.0f,
            1.0f + x, 1.0f + y, 1.0f + z,       1.0f, 0.0f
        };

        for (int v = 0; v < 20; v++)
        {
            this->vertices.push_back(top[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.zNeg)
    {
        float front[] = {
            1.0f + x, -1.0f + y, -1.0f + z,     0.0f, 0.0f,
            -1.0f + x, -1.0f + y, -1.0f + z,    1.0f, 0.0f,
            -1.0f + x, 1.0f + y, -1.0f + z,     1.0f, 1.0f,
            1.0f + x, 1.0f + y, -1.0f + z,      0.0f, 1.0f
        };

        for (int v = 0; v < 20; v++)
        {
            this->vertices.push_back(front[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 1);
        indices.push_back(glNextIndex + 2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex + 2);
        indices.push_back(glNextIndex + 3);

        glNextIndex += 4;
    }
    if (faces.zPos)
    {
        float back[] = {
            -1.0f + x, -1.0f + y, 1.0f + z,     0.0f, 0.0f,
            1.0f + x, -1.0f + y, 1.0f + z,      1.0f, 0.0f,
            1.0f + x, 1.0f + y, 1.0f + z,       1.0f, 1.0f,
            -1.0f + x, 1.0f + y, 1.0f + z,      0.0f, 1.0f
        };

        for (int v = 0; v < 20; v++)
        {
            this->vertices.push_back(back[v]);
        }

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex+1);
        indices.push_back(glNextIndex+2);

        indices.push_back(glNextIndex);
        indices.push_back(glNextIndex+2);
        indices.push_back(glNextIndex+3);

        glNextIndex += 4;
    }

}
