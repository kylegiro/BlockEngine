#include "Chunk.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Chunk::Chunk(int x, int y, int z, Texture& texture) : x(x), y(y), z(z), texture(texture)
{
}

Chunk::~Chunk()
{
}

void Chunk::render(Shader& shader)
{
    // bind texture
    glUniform1i(shader.getUniformLocation("tex"), 0);
    texture.bind();

    // transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(CHUNK_SIZE * (float)getX(), CHUNK_SIZE * (float)getY(), CHUNK_SIZE * (float)getZ()));
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::rebuildMesh()
{
    vertices.clear();
    indices.clear();

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (blocks[x][y][z].getType() == Block::Type::AIR)
					continue;

                FaceRenderFlags faces = {true, true, true, true, true, true};
                if (x > 0)
                    faces.xNeg = !blocks[x-1][y][z].isOpaque();
                if (x < CHUNK_SIZE-1)
                    faces.xPos = !blocks[x+1][y][z].isOpaque();
                if (y > 0)
                    faces.yNeg = !blocks[x][y-1][z].isOpaque();
                if (y < CHUNK_SIZE-1)
                    faces.yPos = !blocks[x][y+1][z].isOpaque();
                if (z > 0)
                    faces.zNeg = !blocks[x][y][z-1].isOpaque();
                if (z < CHUNK_SIZE-1)
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

bool Chunk::isLoaded()
{
    return loadedFlag;
}

bool Chunk::isSetup()
{
    return setupFlag;
}

void Chunk::setup()
{
    // noise generation here?
    setupFlag = true;
}

void Chunk::unload()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            delete[] blocks[i][j];
        }
        delete[] blocks[i];
    }
    delete[] blocks;
}

void Chunk::load()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    blocks = new Block * *[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        blocks[i] = new Block * [CHUNK_SIZE];
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            blocks[i][j] = new Block[CHUNK_SIZE];
        }
    }

    loadedFlag = true;
}

int Chunk::getX()
{
    return x;
}

int Chunk::getY()
{
    return y;
}

int Chunk::getZ()
{
    return z;
}

bool Chunk::shouldRender()
{
    return true;
}

void Chunk::addBlockToMesh(int x, int y, int z, FaceRenderFlags faces)
{
    if (faces.xNeg)
    {
        float left[] = {
            -0.5f + x, -0.5f + y, -0.5f + z,    0.0f, 0.0f,
            -0.5f + x, -0.5f + y, 0.5f + z,     1.0f, 0.0f,
            -0.5f + x, 0.5f + y, 0.5f + z,      1.0f, 1.0f,
            -0.5f + x, 0.5f + y, -0.5f + z,     0.0f, 1.0f
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
           0.5f + x, -0.5f + y, 0.5f + z,      0.0f, 0.0f,
           0.5f + x, -0.5f + y, -0.5f + z,     1.0f, 0.0f,
           0.5f + x, 0.5f + y, -0.5f + z,      1.0f, 1.0f,
           0.5f + x, 0.5f + y, 0.5f + z,       0.0f, 1.0f
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
            -0.5f + x, -0.5f + y, -0.5f + z,    0.0f, 1.0f,
            0.5f + x, -0.5f + y, -0.5f + z,     1.0f, 1.0f,
            0.5f + x, -0.5f + y, 0.5f + z,      1.0f, 0.0f,
            -0.5f + x, -0.5f + y, 0.5f + z,     0.0f, 0.0f
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
            0.5f + x, 0.5f + y, -0.5f + z,      1.0f, 1.0f,
            -0.5f + x, 0.5f + y, -0.5f + z,     0.0f, 1.0f,
            -0.5f + x, 0.5f + y, 0.5f + z,      0.0f, 0.0f,
            0.5f + x, 0.5f + y, 0.5f + z,       1.0f, 0.0f
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
            0.5f + x, -0.5f + y, -0.5f + z,     0.0f, 0.0f,
            -0.5f + x, -0.5f + y, -0.5f + z,    1.0f, 0.0f,
            -0.5f + x, 0.5f + y, -0.5f + z,     1.0f, 1.0f,
            0.5f + x, 0.5f + y, -0.5f + z,      0.0f, 1.0f
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
            -0.5f + x, -0.5f + y, 0.5f + z,     0.0f, 0.0f,
            0.5f + x, -0.5f + y, 0.5f + z,      1.0f, 0.0f,
            0.5f + x, 0.5f + y, 0.5f + z,       1.0f, 1.0f,
            -0.5f + x, 0.5f + y, 0.5f + z,      0.0f, 1.0f
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
