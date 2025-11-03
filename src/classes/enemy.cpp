//
// Created by griff on 11/2/2025.
//

#include "enemy.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>

#include "player.h"
#include "stb_image.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
std::vector<std::vector<int>> bfs(int xTar, int yTar, int xStart, int yStart, std::vector<std::vector<int>> &currentMap) {
    std::vector<std::vector<int>> path;

    // Get map dimensions correctly
    int height = currentMap.size();
    int width = (height > 0) ? currentMap[yTar].size() : 0;

    // Check if start or target is invalid
    if (xStart < 0 || yStart < 0 || yStart >= height || xStart >= width ||
        xTar < 0 || yTar < 0 || yTar >= height || xTar >= width ||
        currentMap[yStart][xStart] != 0 || currentMap[yTar][xTar] != 0) {
        return path; // Return empty path
    }

    // BFS setup
    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;
    std::set<std::pair<int, int>> visited;

    q.push({xStart, yStart});
    visited.insert({xStart, yStart});
    parent[{xStart, yStart}] = {-1, -1};

    // Directions: up, down, left, right
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    bool found = false;

    while (!q.empty() && !found) {
        auto [x, y] = q.front();
        q.pop();

        if (x == xTar && y == yTar) {
            found = true;
            break;
        }

        // Check all 4 neighbors
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Check if neighbor is valid and not visited
            if (nx >= 0 && nx < width &&
                ny >= 0 && ny < height &&
                currentMap[ny][nx] == 0 &&
                visited.find({nx, ny}) == visited.end()) {

                q.push({nx, ny});
                visited.insert({nx, ny});
                parent[{nx, ny}] = {x, y};
            }
        }
    }

    // Reconstruct path if target was found
    if (found) {
        std::pair<int, int> current = {xTar, yTar};
        while (current.first != -1) {
            path.push_back({current.first, current.second});
            current = parent[current];
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}

std::vector<int> hunt(int x, int y, enemy* self, std::vector<std::vector<int>> &currentMap) {

    std::vector<std::vector<int>> path = bfs(x,y,self->world.gridX(),self->world.gridY(),currentMap);
    if (path.size() == 1) {
        return path[0];
    }
    std::vector<int> step = {path[1][0] - path[0][0],path[1][1] - path[0][1]};
    if (abs(step[0]) > abs(step[1])) {
        step = {step[0],0};
    }
    else {
        step = {0,step[1]};
    }
    step[0] = (step[0] == 0)? 0: step[0]/abs(step[0]) ;
    step[1] = (step[1] == 0)? 0: step[1]/abs(step[1]) ;

    self->world.gridPos(step);
    return step;
}






enemy::enemy(grid gameSpace, const std::string &vsPath, const std::string &fsPath, const std::string &texPath, int x, int y) :
world(gameSpace),
Shader(vsPath.c_str(), fsPath.c_str()) {

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

enemy::~enemy() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteTextures(1, &this->textureID);
}

void enemy::draw(int x, int y) {
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
void enemy::draw() {
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

void enemy::move(int x, int y, std::vector<std::vector<int>> currentMap) {
    std::vector<int> movement = hunt(x, y, this, currentMap);

}
