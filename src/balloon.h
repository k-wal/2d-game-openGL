#include "main.h"

#ifndef BALLOON_H
#define BALLOON_H


class Balloon {
public:
    Balloon() {}
    Balloon(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int is_exist;
    double speed_y;
    double speed_x;
    double acc_y;
    float r;
    bounding_box_t bound;
    void update_bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
