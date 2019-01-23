#include "main.h"

#ifndef CIRCLE_H
#define CIRCLE_H


class Circle {
public:
    Circle() {}
    Circle(float x, float y,color_t color);
    glm::vec3 position;
    float r;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // BEAM_H
