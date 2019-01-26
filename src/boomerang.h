#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H


class Boomerang {
public:
    Boomerang() {}
    Boomerang(float x, float y,color_t color);
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
    float a,b,center_x,center_y;
    int is_exist;
    float theta;
    bounding_box_t bound;
    void update_bounding_box();
private:
    VAO *object;
    VAO *end1;
    VAO *end2;
};

#endif // ZAPPER_H
