#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet
{
public:
    Magnet() {}
    Magnet(float x,int is_top, color_t color_red,color_t color_grey);
    glm::vec3 position;
    int is_exist;
    int ticks_left;
    float acc_x;
    float acc_y;
    float speed_x;
    float r;
    float top_y;
    float bottom_y;
    int is_top;
    int rotation;
    void draw(glm::mat4 VP);
    // void set_position(float x, float y);
    void tick();
    bounding_box_t bound;
private:
    VAO *leg1;
    VAO *leg2;
    VAO *head;
};

#endif // MAGNET_H
