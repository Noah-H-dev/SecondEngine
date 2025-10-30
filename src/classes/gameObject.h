//
// Created by griff on 10/23/2025.
//

#ifndef OPENGLCLION_GAMEOBJECT_H
#define OPENGLCLION_GAMEOBJECT_H


class shader;

class gameObject {
public:
    virtual ~gameObject() = default;
    virtual void draw(int x, int y){}
};


#endif //OPENGLCLION_GAMEOBJECT_H