//
// Created by griff on 10/23/2025.
//

#ifndef OPENGLCLION_STATICOBJECT_H
#define OPENGLCLION_STATICOBJECT_H
#include <string>
#include <vector>

#include "gameObject.h"
#include "grid.h"

class staticObject final : public gameObject{
public:
    staticObject(const grid gameSpace, const std::string &vsPath, const std::string &fsPath, const std::string &texPath);
    ~staticObject() override;
    void draw(int x, int y) override;

    unsigned int VAO, VBO, textureID;

    shader* Shader;
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


#endif //OPENGLCLION_STATICOBJECT_H