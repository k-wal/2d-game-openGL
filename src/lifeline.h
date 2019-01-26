#include "main.h"

#ifndef LIFELINE_H
#define LIFELINE_H


class Lifeline {
public:
    Lifeline() {}
    Lifeline(float x, float y,color_t color);
    glm::vec3 position;
    float r1;
    float r2;
    float speed_x;
    float speed_y;
    float acc_x;
    float acc_y;
    int is_exist;
    int rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void update_bounding_box();
    bounding_box_t bound;
private:
    VAO *inside_square;
    VAO *outside_square;
};

#endif // LIFELINE_H