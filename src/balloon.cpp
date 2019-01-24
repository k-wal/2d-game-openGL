#include "balloon.h"
#include "main.h"

Balloon::Balloon(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->acc_y = 0.003;
    this->speed_y = 0.1;
    this->speed_x = 0.05;
    this->is_exist = 1;
    this->r=0.2f;
    
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->bound.width = this->r;
    this->bound.height = this->r;
    this->bound.rotation = 0;


    int n=100;
    float pi=3.14f;
    GLfloat g_vertex_buffer_data[9*n];
    GLfloat temp[3*n];
    int i;
    for(i=0; i<n; i++)
    {
        float x = this->r * cos(2*pi*i/n);
        float y = this->r * sin(2*pi*i/n);
        temp[3*i]= x;
        temp[3*i +1]= y;
        temp[3*i +2]= 0.0f;
    }
    for(i=0; i<n; i++)
    {
        //center
        g_vertex_buffer_data[9*i] = 0.0f;
        g_vertex_buffer_data[9*i+1] = 0.0f;
        g_vertex_buffer_data[9*i+2] = 0.0f;
        
        //first vertex
        g_vertex_buffer_data[9*i+3] = temp[3*i];
        g_vertex_buffer_data[9*i+4] = temp[3*i+1];
        g_vertex_buffer_data[9*i+5] = temp[3*i+2];

        //second vertex
        g_vertex_buffer_data[9*i+6] = temp[(3*i+3)%(3*n)];
        g_vertex_buffer_data[9*i+7] = temp[(3*i+4)%(3*n)];
        g_vertex_buffer_data[9*i+8] = temp[(3*i+5)%(3*n)];
        
    }
    
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    
    this->object = create3DObject(GL_TRIANGLES, n*3, g_vertex_buffer_data, color, GL_FILL);
    
}

void Balloon::draw(glm::mat4 VP)
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

}


void Balloon::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Balloon::tick()
{
    if(this->is_exist==0)
        return;
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;
    this->speed_y -= this->acc_y;
    if(this->position.y<=-3)
    {
        this->is_exist=0;
    }
    this->update_bounding_box();
}

void Balloon::update_bounding_box()
{
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->bound.width = this->r;
    this->bound.height = this->r;
    this->bound.rotation = this->rotation;
}