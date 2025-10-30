//
// Created by griff on 10/20/2025.
//

#ifndef OPENGLCLION_GAME_H
#define OPENGLCLION_GAME_H
#include <map>
#include <string>
#include <vector>


#include "grid.h"
#include "player.h"

#include "staticObject.h"


struct objectData {
    std::string shaderPaths[2];
    std::string texturePath;
    objectData(const std::string &vsPath, const std::string &fsPath, const std::string &texPath) {
        shaderPaths[0] = vsPath;
        shaderPaths[1] = fsPath;
        texturePath = texPath;
    }
};
enum gameState {
    MAIN_MENU,
    GAME_MENU,
    MAP_EDITOR,
    IN_GAME
};

class game {
    bool ready = true;
    std::vector<int> lastPos;
public:
    game(int tile, int scrWidth, int scrHeight);
    ~game() = default;
    void init();
    void readMap(const std::string& mapPath);
    void idParser();
    void update(int key, int action);
    void render();

    grid gameSpace;
    player* Player;
    gameState currentState = gameState::IN_GAME;
    std::map<int, objectData> staticObjectDataMap;
    std::vector<staticObject> staticGameObjects;
    std::vector<std::vector<int>> currentMap;

};


#endif //OPENGLCLION_GAME_H