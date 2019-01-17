#include "main.h"

#ifndef ZAPPER_H
#define ZAPPER_H


class Zapper {
public:
    Zapper() {}
    Zapper(float x, float y, float rotation,float width,color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float width;
    float thickness;
    bounding_box_t bound;
private:
    VAO *object;
};

#endif // ZAPPER_H
