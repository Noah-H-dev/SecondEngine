//
// Created by griff on 10/23/2025.
//

#ifndef OPENGLCLION_GRID_H
#define OPENGLCLION_GRID_H
#include <vector>

struct grid {
    int totalWidth;
    int totalHeight;
    int tileSize;
    int xPos, yPos;
    grid(int scrWidth, int scrHeight, int tile) {
        totalWidth = scrWidth;
        totalHeight = scrHeight;
        tileSize = tile;
    }
    void resize(const int x, const int y) {
        totalWidth = x;
        totalHeight = y;
    }
    std::vector<int> gridPos(const int x, const int y) {
        xPos = x*tileSize;
        yPos = y*tileSize;
        return {x*tileSize, y*tileSize};
    }
    void gridPos(const std::vector<int> &movement) {
        xPos += tileSize*movement[0];
        yPos += tileSize*movement[1];
    }
    std::vector<int> gridPos() {
        return {xPos, yPos};
    }
    int gridPos(const int loc) const {
        return loc*tileSize;
    }
    /*
     *returns the pure x value unscaled by the tilesize
     */
    int gridX() const {
        return xPos/tileSize;
    }
    /*
     *returns the pure x value unscaled by the tilesize
     */
    int gridY() const {
        return yPos/tileSize;
    }
};




#endif //OPENGLCLION_GRID_H