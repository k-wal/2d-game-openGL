#include "coin.h"
#include "main.h"

Coin::Coin(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->r = 0.1;
    this->bound.width = this->r;
    this->bound.height = this->r;
    this->rotation=0;
    
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
        -this->r, this->r, 0.0f,
        this->r, this->r, 0.0f,
        this->r, -this->r, 0.0f,
        -this->r, this->r, 0.0f,
        -this->r, -this->r, 0.0f,
        this->r, -this->r, 0.0f,
        };
    
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color , GL_FILL);
}

void Coin::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    //rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Coin::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Coin::tick()
{
    this->rotation++;
    this->rotation%=360;        
}
