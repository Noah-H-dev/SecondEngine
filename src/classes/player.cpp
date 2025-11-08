//
// Created by griff on 10/23/2025.
//

#include "player.h"
//
// Created by griff on 10/23/2025.
//

#include "staticObject.h"

#include <iostream>

#include "stb_image.h"
#include "grid.h"
#include "shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

player::player(grid gameSpace, const std::string &vsPath, const std::string &fsPath, const std::string &texPath, int x, int y) :
world(gameSpace),
Shader(vsPath.c_str(), fsPath.c_str()),
playerAttrib(10){

    world.gridPos(x,y);
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    // Bind VAO first - it will capture all subsequent state
    glBindVertexArray(this->VAO);

    // Setup VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->container.size()*sizeof(float), this->container.data(), GL_STATIC_DRAW);

    // Configure vertex attributes (while VAO is bound)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texPath.c_str(), &width, &height,&nrChannels, 0);
    if (data)
    {
        GLint format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    this->Shader.use();
    this->Shader.setInt("tex", 0);  // link sampler uniform to GL_TEXTURE0

    glBindTexture(GL_TEXTURE_2D, 0);       // Also unbind texture for cleanliness
    glBindBuffer(GL_ARRAY_BUFFER, 0);      // Unbind VBO - safe to do before VAO
    glBindVertexArray(0);
}

player::~player() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteTextures(1, &this->textureID);
}

void player::draw(int x, int y) {
    world.gridPos(x, y);

    this->Shader.use();

    glm::mat4 projection = glm::ortho(0.0f, (float)world.totalWidth, 0.0f, (float)world.totalHeight, -1.0f, 1.0f);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(world.xPos, world.yPos, 0.0f));
    model = glm::scale(model, glm::vec3((float) world.tileSize,(float) world.tileSize, 1.0f));


    this->Shader.setMat4("projection", projection);
    this->Shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);


    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

}
void player::draw() {
    this->Shader.use();

    glm::mat4 projection = glm::ortho(0.0f, (float)world.totalWidth, 0.0f, (float)world.totalHeight, -1.0f, 1.0f);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(world.xPos, world.yPos, 0.0f));
    model = glm::scale(model, glm::vec3((float) world.tileSize,(float) world.tileSize, 1.0f));


    this->Shader.setMat4("projection", projection);
    this->Shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);


    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void player::move(int key, int action) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        this->world.yPos += world.tileSize;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        this->world.yPos -= world.tileSize;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        this->world.xPos -= world.tileSize;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        this->world.xPos += world.tileSize;
    }
}
