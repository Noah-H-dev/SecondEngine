//
// Created by griff on 11/2/2025.
//

#ifndef OPENGLCLION_ENEMY_H
#define OPENGLCLION_ENEMY_H
#include <string>
#include <vector>

#include "grid.h"
#include "attributes.h"
#include "shader.h"


class player;

class enemy {
public:

    enemy(const grid gameSpace, const std::string &vsPath, const std::string &fsPath, const std::string &texPath, int x , int y);
    ~enemy();
    void draw(int x, int y);
    void draw();
    void move(int x, int y, std::vector<std::vector<int>> currentMap);



    unsigned int VAO, VBO, textureID;
    shader Shader;
    grid world;
    attributes enemyAttrib;
    std::vector<float> container = { // pivot bottom left
        1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
    };
};


#endif //OPENGLCLION_ENEMY_H