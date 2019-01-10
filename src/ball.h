#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed_y;
    double speed_x;
    double acc_y;
    double acc_x;
    float r;
    void left_click();
    void right_click();
private:
    VAO *object;
};

#endif // BALL_H
