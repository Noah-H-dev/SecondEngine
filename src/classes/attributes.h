//
// Created by griff on 11/6/2025.
//

#ifndef OPENGLCLION_ATTRIBUTES_H
#define OPENGLCLION_ATTRIBUTES_H
#include <algorithm>

struct attributes {
    double maxHealth;
    double currentHealth;
    int strength;
    int stamina;

    explicit attributes(double maxH) {
        maxHealth = maxH;
        currentHealth = maxHealth;
        strength = 1;
        stamina = 1;
    }
 //impliment inventory as a vector of ints (itemIDs)
    // -> impliment items as a class which have an ID
    // -> create a global ID bank in the game class containing all item IDS
 //feats as an array of feat classes
};

#endif //OPENGLCLION_ATTRIBUTES_H