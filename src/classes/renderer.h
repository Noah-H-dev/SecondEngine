//
// Created by griff on 10/25/2025.
//

#ifndef OPENGLCLION_RENDERER_H
#define OPENGLCLION_RENDERER_H
#include <map>
#include <vector>

#include "GLFW/glfw3.h"


struct grid;
class staticObject;
class renderer {
public:
    static void renderGame(const std::vector<std::vector<int>> &currentMap,std::vector<staticObject> &background);
    void renderMainMenu();
    void renderGameMenu();
    void renderMapEditor();
};


#endif //OPENGLCLION_RENDERER_H