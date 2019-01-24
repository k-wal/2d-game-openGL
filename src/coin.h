#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y,color_t color);
    glm::vec3 position;
    float r;
    int rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bound;
private:
    VAO *object;
};

#endif // COIN_H
