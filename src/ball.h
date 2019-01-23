#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color,color_t color_fire);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void draw_fire(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed_y;
    double speed_x;
    double acc_y;
    double acc_x;
    float step_length;
    float r;

    //in ring = 1 if the player is currently inside a semi-circular ring, 0 if not
    int in_ring;
    
    // cur_jump_state : 1 if space is pressed, 0 otherwise
    float cur_jump_state;
    void left_click();
    void right_click();
    void jump();
    bounding_box_t bound;
    void update_bounding_box();
private:
    VAO *object;
    VAO *fire;
};

#endif // BALL_H
