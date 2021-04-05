#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Block.h"
#include "Texture.h"

Block::Block(float r, float g, float b, float a, Texture& texture) : r(r), g(g), b(b), a(a), texture(texture)
{
    float vertices[] = {
        // front face
        1.0f, -1.0f, -1.0f,     0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,     1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f,
        // back face
        -1.0f, -1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,      1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,       1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,      0.0f, 1.0f,
         // right face
        1.0f, -1.0f, 1.0f,      0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,      1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,       0.0f, 1.0f,
         // left face
        -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,     0.0f, 1.0f,
        // bottom face
        -1.0f, -1.0f, -1.0f,    0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,      1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     0.0f, 0.0f,
        // top face
        1.0f, 1.0f, -1.0f,      1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,       1.0f, 0.0f
    };
    unsigned int indices[] = {
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void Block::render(Shader& shader)
{
    // bind texture
    glUniform1i(shader.getUniformLocation("tex"), 0);
    texture.bind();

    // transformations
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}