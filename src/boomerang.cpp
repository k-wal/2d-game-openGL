#include "boomerang.h"
#include "main.h"

Boomerang::Boomerang(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->width = 0.4;
    this->thickness = 0.04;
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->bound.width = this->thickness;
    this->bound.height = this->width;
    this->bound.rotation = this->rotation;
    this->end_height = 0.08;
    this->end_width = 0.04;
    this->a = 4.0f;
    this->b = 2.0f;
    this->center_x = x-a;
    this->center_y = y;
    this->is_exist = 1;
    this->theta = 0;

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    /*
    static const GLfloat vertex_buffer_data[] = {
        -this->width, this->thickness, 0.0f,
        this->width, this->thickness, 0.0f,
        this->width, -this->thickness, 0.0f,
        -this->width, this->thickness, 0.0f,
        -this->width, -this->thickness, 0.0f,
        this->width, -this->thickness, 0.0f,
        };
    */
    static const GLfloat vertex_buffer_data[] = {
        -this->width, this->thickness, 0.0f,
        this->width, this->thickness, 0.0f,
        this->width, -this->thickness, 0.0f,
        -this->width, this->thickness, 0.0f,
        -this->width, -this->thickness, 0.0f,
        this->width, -this->thickness, 0.0f,
        };

    static const GLfloat vertex_buffer_data_end1[] = {
        -this->width-this->end_width, this->end_height, 0.0f,
        -this->width, this->end_height, 0.0f,
        -this->width, -this->end_height, 0.0f,
        -this->width-this->end_width, this->end_height, 0.0f,
        -this->width-this->end_width, -this->end_height, 0.0f,
        -this->width, -this->end_height, 0.0f,
        };
    
    static const GLfloat vertex_buffer_data_end2[] = {
        this->width, this->end_height, 0.0f,
        this->width+this->end_width, this->end_height, 0.0f,
        this->width+this->end_width, -this->end_height, 0.0f,
        this->width, this->end_height, 0.0f,
        this->width, -this->end_height, 0.0f,
        this->width+this->end_width, -this->end_height, 0.0f,
        };
    
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
    this->end1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_end1, COLOR_INDIGO , GL_FILL);
    this->end2 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data_end2, COLOR_INDIGO , GL_FILL);

}

void Boomerang::draw(glm::mat4 VP)
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
    draw3DObject(this->object);
    draw3DObject(this->end1);
    draw3DObject(this->end2);
}

void Boomerang::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Boomerang::tick()
{
    if(this->is_exist==0)
        return;
    float screen_down = -3;
    this->rotation+=3;
    this->theta++;
    this->position.x = this->center_x + this->a*cos(this->theta * M_PI/180.0);
    this->position.y = this->center_y + this->b*sin(this->theta * M_PI/180.0);
    if(this->position.y - this->width <= screen_down)
        this->is_exist=0;
    this->update_bounding_box();

}

void Boomerang::update_bounding_box()
{
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->bound.rotation = this->rotation;
    this->bound.width = this->thickness;
    this->bound.height = this->width;
    this->bound.rotation = this->rotation;
}