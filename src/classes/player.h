//
// Created by griff on 10/23/2025.
//

#ifndef OPENGLCLION_PLAYER_H
#define OPENGLCLION_PLAYER_H
#include <string>

#include "gameObject.h"
#include "grid.h"
#include  "shader.h"

class player : public gameObject {
public:
    player(const grid gameSpace, const std::string &vsPath, const std::string &fsPath, const std::string &texPath, int x , int y);
    ~player() override;
    void draw(int x, int y) override;
    void draw();
    void move(int key, int action);


    unsigned int VAO, VBO, textureID;

    shader Shader;
    grid world;
    std::vector<float> container = { // pivot bottom left
        1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
    };
};


#endif //OPENGLCLION_PLAYER_H