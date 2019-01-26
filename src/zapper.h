#include "main.h"

#ifndef ZAPPER_H
#define ZAPPER_H


class Zapper {
public:
    Zapper() {}
    Zapper(float x, float y, float rotation,float width,color_t color);
    glm::vec3 position;
    float rotation;
    float r;
    float end_height;
    float end_width;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float width;
    float thickness;
    bounding_box_t bound;
private:
    VAO *object;
    VAO *end1;
    VAO *end2;
};

#endif // ZAPPER_H
