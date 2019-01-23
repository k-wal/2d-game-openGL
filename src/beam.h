#include "main.h"

#ifndef BEAM_H
#define BEAM_H


class Beam {
public:
    Beam() {}
    Beam(float x, float y, float width,color_t color,float thickness);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float width;
    float thickness;
    int direction;
    float speed;
    void update_bounding_box();
    bounding_box_t bound;
private:
    VAO *object;
};

#endif // BEAM_H
