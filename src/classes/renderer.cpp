//
// Created by griff on 10/25/2025.
//

#include "renderer.h"

#include <iostream>

#include "grid.h"
#include "staticObject.h"

void renderer::renderGame(const std::vector<std::vector<int>> &currentMap, std::vector<staticObject>& background) {
    for (int i = 0; i < currentMap.size() - 1; i++) {
        for (int j = 0; j < currentMap[i].size(); j++) {
            if (currentMap[i][j] == -1) {
                continue; // is a whitespace
            }
            else {
                background.at(currentMap[i][j]).draw(j,i);
            }
        }
    }
}
