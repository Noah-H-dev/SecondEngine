//
// Created by griff on 10/20/2025.
//
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "game.h"

#include "renderer.h"



game::game(int tile, int scrWidth, int scrHeight):gameSpace(scrWidth,scrHeight,tile){
    init();
    idParser();
    enemies.emplace_back(this->gameSpace, "../src/shaders/static.vs", "../src/shaders/static.fs", "../src/textures/enemy.png", 15, 15);
    readMap("../src/maps/default.txt");
}

void game::close() {
    for (auto& c: staticGameObjects) {
        glDeleteVertexArrays(1, &c.VAO);
        glDeleteBuffers(1, &c.VBO);
        glDeleteTextures(1, &c.textureID);
    }
}

void game::init() { std::ifstream inputFile("../src/ID.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open texture ID file" << std::endl;
    }
    std::string texIDs( (std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();
    texIDs.erase(std::remove(texIDs.begin(), texIDs.end(), '\n'), texIDs.end());
    int start = 0;
    int head = 0;
    int id = 0;
    std::string currentShader = "none";
    std::string currentTexture;

    for (char c: texIDs) {
         switch (c) {
             case '/':
                 start = head + 1;
                 break;
             case '|':
                 currentShader = texIDs.substr(start, head - start);
                 start = head + 1;
                 break;
             case ' ':
                 id = std::stoi(texIDs.substr(start, head - start));
                 start = head + 1;
                 break;
             case ';':
                 currentTexture = texIDs.substr(start,head-start);

                 staticObjectDataMap.insert(std::pair(id, objectData(
                     "../src/shaders/" + currentShader + ".vs",
                     "../src/shaders/" + currentShader + ".fs",
                     "../src/textures/" + currentTexture)));
                 break;
             default: break;
        }
        head++;
    }
}

void game::readMap(const std::string& mapPath) {
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        std::cerr << "Could not open file!\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (const char c : line) {
            if (std::isdigit(c)) {  // only process digits
                row.push_back(c - '0');  // convert char -> int
            }
            else {
                row.push_back(-1);
            }
        }
        this->currentMap.push_back(row);
    }
    std::reverse(this->currentMap.begin(), this->currentMap.end() - 1);
    file.close();
}

void game::idParser() {
    for (const auto& c: staticObjectDataMap) {
        staticGameObjects.emplace_back(this->gameSpace,c.second.shaderPaths[0], c.second.shaderPaths[1], c.second.texturePath);
    }
}

void game::update(int key, int action) {
    if (ready == true){
        lastPos = Player->world.gridPos();
        Player->move(key,action);
        int mapPos = currentMap[Player->world.gridY()][Player->world.gridX()];
        if (mapPos == 1 || mapPos == -1){
            Player->world.xPos = lastPos[0];
            Player->world.yPos = lastPos[1];
        }
        for (enemy &e: enemies) {

            e.move(Player->world.gridX(), Player->world.gridY(),currentMap);
        }
        ready = false;
    }
    else if (ready == false && action == GLFW_RELEASE) {
        ready = true;
    }
}

void game::render() {
    switch (currentState) {
        case gameState::IN_GAME:{
            renderer::renderGame(currentMap,staticGameObjects);


            Player->draw();
            for (enemy &e: enemies) {
                e.draw();
            }
        }
            break;
        default: break;
    }
}
