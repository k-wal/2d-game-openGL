#include "ring.h"
#include "main.h"

GLfloat temp1[100000];
GLfloat temp2[100000];
 
Ring::Ring(float x, float y,color_t color1,color_t color2)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->r1 = 1.5;
    this->r2 = 1.3;
    this->thickness = this->r1 - this->r2;
    //this->width = width;
    this->bound.x = position.x;
    this->bound.y = position.y;
    this->bound.width = this->r1;
    this->bound.height = this->r1;
    //this->bound.rotation = this->rotation;
    int n=360;
    float pi = M_PI;
    GLfloat g_vertex_buffer_data1[(9*n)/2];
    //GLfloat temp1[100000];
    GLfloat g_vertex_buffer_data2[(9*n)/2];
    //GLfloat temp2[100000];
    int i;
    for(i=0; i<n/2; i++)
    {
        float x = this->r1 * cos(2*pi*i/n);
        float y = this->r1 * sin(2*pi*i/n);
        temp1[3*i]= x;
        temp1[3*i +1]= y;
        temp1[3*i +2]= 0.0f;
        x = this->r2 * cos(2*pi*i/n);
        y = this->r2 * sin(2*pi*i/n);
        temp2[3*i]= x;
        temp2[3*i +1]= y;
        temp2[3*i +2]= 0.0f;
           
    }
    for(i=0; i<n/2; i++)
    {
        //center
        g_vertex_buffer_data1[9*i] = 0.0f;
        g_vertex_buffer_data1[9*i+1] = 0.0f;
        g_vertex_buffer_data1[9*i+2] = 0.0f;
        
        //first vertex
        g_vertex_buffer_data1[9*i+3] = temp1[3*i];
        g_vertex_buffer_data1[9*i+4] = temp1[3*i+1];
        g_vertex_buffer_data1[9*i+5] = temp1[3*i+2]; //0.0f; 

        //second vertex
        g_vertex_buffer_data1[9*i+6] = temp1[(3*i+3)%(3*n)];
        g_vertex_buffer_data1[9*i+7] = temp1[(3*i+4)%(3*n)];
        g_vertex_buffer_data1[9*i+8] = temp1[(3*i+5)%(3*n)]; //0.0f; 
        
        //center
        g_vertex_buffer_data2[9*i] = 0.0f;
        g_vertex_buffer_data2[9*i+1] = 0.0f;
        g_vertex_buffer_data2[9*i+2] = 0.0f;
        
        //first vertex
        g_vertex_buffer_data2[9*i+3] = temp2[3*i];
        g_vertex_buffer_data2[9*i+4] = temp2[3*i+1];
        g_vertex_buffer_data2[9*i+5] = temp2[3*i+2]; //0.0f;

        //second vertex
        g_vertex_buffer_data2[9*i+6] = temp2[(3*i+3)%(3*n)];
        g_vertex_buffer_data2[9*i+7] = temp2[(3*i+4)%(3*n)];
        g_vertex_buffer_data2[9*i+8] = temp2[(3*i+5)%(3*n)];  //0.0f; 
        
    }
    
    
    this->object1 = create3DObject(GL_TRIANGLES, (n*3)/2, g_vertex_buffer_data1, color1 , GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, (n*3)/2, g_vertex_buffer_data2, color2 , GL_FILL);

}

void Ring::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    //rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
}

void Ring::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Ring::tick()
{
  return;
}
