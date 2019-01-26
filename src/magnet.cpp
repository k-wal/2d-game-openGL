#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x,int is_top, color_t color_red, color_t color_grey)
{
    this->is_top = is_top;
    this->top_y = 3.3;
    this->bottom_y = -3.2;
    this->rotation = 0;
    this->is_exist = 1;
    this->speed_x = 0.01;
    this->acc_y = 0.001;
    float y;
    if(this->is_top==1)
    {
        y = this->top_y;
        this->rotation=180;
    }
    else
        y = this->bottom_y;

    this->position = glm::vec3(x, y, 0);
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->r = 0.1;
    this->bound.width = this->r;
    this->bound.height = this->r;
    
  float leg_width = 0.1;
  float head_width = 0.5;
  float leg_height = 0.05;
  float head_height = 0.1;
  float x_1_start = 0;
  float x_1_end = leg_width;
  float x_2_start = head_width-leg_width;
  float x_2_end = head_width;
  float y_head_start = 0;
  float y_head_end = head_height;
  float y_leg_start = head_height;
  float y_leg_end = head_height+leg_height;


 static const GLfloat vertex_buffer_data_leg_1[] = {
     x_1_start, y_leg_start, 0.0f,
     x_1_end, y_leg_start, 0.0f,
     x_1_start, y_leg_end, 0.0f,
     x_1_start, y_leg_end, 0.0f,
     x_1_end, y_leg_end, 0.0f,
     x_1_end, y_leg_start, 0.0f,
     };

 static const GLfloat vertex_buffer_data_leg_2[] = {
     x_2_start, y_leg_start, 0.0f,
     x_2_end, y_leg_start, 0.0f,
     x_2_start, y_leg_end, 0.0f,
     x_2_start, y_leg_end, 0.0f,
     x_2_end, y_leg_end, 0.0f,
     x_2_end, y_leg_start, 0.0f,
     };

 static const GLfloat vertex_buffer_data_head[] = {
     0 , y_head_start, 0.0f,
     head_width, y_head_start, 0.0f,
     0, y_head_end, 0.0f,
     0, y_head_end, 0.0f,
     head_width, y_head_end, 0.0f,
     head_width, y_head_start, 0.0f,
     };
        

    this->leg1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_leg_1, color_grey , GL_FILL);
     this->leg2 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_leg_2, color_grey , GL_FILL);
     this->head = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_head, color_red , GL_FILL);

}

void Magnet::draw(glm::mat4 VP)
{
    if(this->is_exist==0)
        return;
    Matrices.model = glm::mat4(1.0f);
    //this->position.x = this->position.y = 0.0; 
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    //rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->leg1);
    draw3DObject(this->leg2);
    draw3DObject(this->head);

}

// void Magnet::set_position(float x, float y)
// {
//     this->position = glm::vec3(x, y, 0);
// }

void Magnet::tick()
{
//    printf("ticking\n");
    this->ticks_left--;
}
