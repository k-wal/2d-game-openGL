#include "lifeline.h"
#include "main.h"

Lifeline::Lifeline(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->r1 = 0.1;
    this->bound.width = this->r1;
    this->bound.height = this->r1;
    this->rotation=0;
    this->r2 = 0.095;
    this->is_exist = 1;

    this->speed_x = -0.025;
    this->speed_y = 0.08;
    this->acc_y = -0.001;

    static const GLfloat vertex_buffer_data_outside[] = {
        -this->r1, this->r1, 0.0f,
        this->r1, this->r1, 0.0f,
        this->r1, -this->r1, 0.0f,
        -this->r1, this->r1, 0.0f,
        -this->r1, -this->r1, 0.0f,
        this->r1, -this->r1, 0.0f,
        };
    static const GLfloat vertex_buffer_data_inside[] = {
        -this->r2, this->r2, 0.0f,
        this->r2, this->r2, 0.0f,
        this->r2, -this->r2, 0.0f,
        -this->r2, this->r2, 0.0f,
        -this->r2, -this->r2, 0.0f,
        this->r2, -this->r2, 0.0f,
        };
    

    this->inside_square = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_inside, color , GL_FILL);
    this->outside_square = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_outside, COLOR_WHITE , GL_FILL);

}

void Lifeline::draw(glm::mat4 VP)
{
    if(this->is_exist==0)
        return;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    //rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->outside_square);
    draw3DObject(this->inside_square);

}

void Lifeline::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Lifeline::tick()
{
    if(this->is_exist==0)
        return;
    float screen_down = -3.0f;
    if(this->position.y - this->bound.height <= screen_down)
    {
        this->is_exist=0;
        return;
    }
    this->rotation++;
    this->rotation%=180;
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;
    this->speed_y += this->acc_y;
    this->update_bounding_box();
}

void Lifeline::update_bounding_box()
{
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->bound.width = this->r1;
    this->bound.height = this->r1;
    
}
