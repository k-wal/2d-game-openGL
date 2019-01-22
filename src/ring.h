#include "main.h"

#ifndef RING_H
#define RING_H


class Ring {
public:
    Ring() {}
    Ring(float x, float y ,color_t color1,color_t color2);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float r1;
    float r2;
    float thickness;
    bounding_box_t bound;
private:
    VAO *object1;
    VAO *object2;
};

#endif // ZAPPER_H
