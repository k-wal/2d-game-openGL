#include "main.h"

#ifndef SEGMENT_H
#define SEGMENT_H


class Segment {
public:
    Segment() {}
    Segment(float x, float y, float rotation,float width,color_t color);
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

#endif // SEGMENT_H
