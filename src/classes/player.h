//
// Created by griff on 10/23/2025.
//

#ifndef OPENGLCLION_PLAYER_H
#define OPENGLCLION_PLAYER_H
#include <string>

#include "grid.h"
#include "attributes.h"
#include "shader.h"

class enemy;
struct atk {
    int damage;
    int impact;
};
class player {
public:
    player(const grid gameSpace, const std::string &vsPath, const std::string &fsPath, const std::string &texPath, int x , int y);
    ~player();
    void draw(int x, int y);
    void draw();
    void move(int key, int action);
    atk attack ();


    unsigned int VAO, VBO, textureID;

    shader Shader;
    grid world;
    attributes playerAttrib;
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