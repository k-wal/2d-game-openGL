#include "main.h"
#include "timer.h"
#include "ball.h"
#include "zapper.h"
#include "coin.h"
#include "platform.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Platform platform1;
vector<Zapper> zappers;
vector<Coin> coins;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_target_x =0;
float camera_location_x=0;
float camera_rotation_angle = 0;
int count_zappers=0;
int count_coins=0;
int zappers_hit=0;
int score=0;
int coins_scored=0;
int life=0;
int hang=0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw()
{
    if(hang>0)
    {
        return;
    }
    
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    //Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(camera_location_x, 0, 3), glm::vec3(camera_target_x, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    platform1.draw(VP);
    //z.draw(VP);
    for(int i=0; i<zappers.size(); i++)
    {
        zappers[i].draw(VP);
    }
    for(int i=0; i<coins.size(); i++)
    {
        coins[i].draw(VP);
    }
}

void create_zapper()
{
    if(count_zappers>=1)
        return;
    int rnum=rand();
    Zapper z=Zapper(5+camera_location_x,rnum%5-2,rnum%120+30,1.4,COLOR_YELLOW);
    zappers.push_back(z);
    count_zappers++;
}

void create_coin()
{
    if(count_coins>=2)
        return;
    int rnum=rand();
    Coin c=Coin(5+camera_location_x,rnum%4-1,COLOR_GOLDEN);
    coins.push_back(c);
    count_coins++;
}

void count_elements()
{
    count_zappers=zappers.size();
    zappers_hit=0;
    for(int i=0; i<zappers.size(); i++)
    {
        Zapper z=zappers[i];
        if(z.position.x<camera_location_x-3)
        {
            count_zappers--;
        }
        if(z.position.x==-100)
        {
            zappers_hit++;
        }
    }
    count_coins=coins.size();
    coins_scored=0;
    for(int i=0; i<coins.size(); i++)
    {
        Coin c=coins[i];
        if(c.position.x<camera_location_x-3)
        {
            count_coins--;
        }
        if(c.position.x==-100)
        {
            coins_scored++;
        }
    }
    //printf("%d\n",coins_scored);
}

void detect_all_collisions()
{
    for(int i=0; i<zappers.size(); i++)
    {
        if(detect_collision_line(zappers[i].bound,ball1.bound))
        {
            zappers[i].position.x = -100;
        }
    }
    for(int i=0; i<coins.size(); i++)
    {
        if(detect_collision_square(coins[i].bound,ball1.bound))
        {
            coins[i].position.x = -100;
        }
    }
}

void tick_input(GLFWwindow *window)
{
    if(hang>0)
    {
        return;
    }
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    //int up  = glfwGetKey(window, GLFW_KEY_UP);
    //int down = glfwGetKey(window, GLFW_KEY_DOWN);
    
    if (left)
    {
        ball1.left_click();
        camera_location_x-=0.01;
        camera_target_x-=0.01;
        detect_all_collisions();
    }
    if(right)
    {
        ball1.right_click();
        camera_location_x+=0.01;
        camera_target_x+=0.01;
        detect_all_collisions();
        create_zapper();
        create_coin();
    }
  
    //    glm::vec3 target (screen_center_x, 0, 0);
    if(space)
    {
        ball1.jump();
        detect_all_collisions();
    }
}


void tick_elements(GLFWwindow *window)
{
    if(hang>0)
    {
        return;
    }
    char title[1000];
    ball1.tick();
    detect_all_collisions();
    count_elements();
    score=coins_scored*5;
    life=5-zappers_hit;
    sprintf(title,"SCORE : %d\t LIFE : %d",score,life);
    glfwSetWindowTitle(window,title);
    //camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1 = Ball(0, 0, COLOR_RED);
    Zapper z = Zapper(1,2,30,1.2,COLOR_YELLOW);
    zappers.push_back(z);
    z = Zapper(4,1,60,1.2,COLOR_YELLOW);
    zappers.push_back(z);
    Coin c = Coin(1.5,0,COLOR_GOLDEN);
    coins.push_back(c);
    c = Coin(3.5,-2,COLOR_GOLDEN);
    coins.push_back(c);
    count_elements();

    platform1 = Platform(500,-1003.05,COLOR_BLACK);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv)
{
    srand(time(0));
    int width  = 600;
    int height = 600;
    char title[1000];

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        
        if (t60.processTick())
        {
            if(hang>0)
            {
                hang--;
            }
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements(window);
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

// returns distance between points (x1,y1) and (x2,y2)
float distance(float x1,float y1, float x2, float y2)
{
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

bool detect_collision_square(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x)  < (a.width + b.width)) &&
           (abs(a.y - b.y)  < (a.height + b.height));
}

//l : bounding box of line
//p : bounding box of player(square)
//returns true if collision detected b/w line and box
bool detect_collision_line(bounding_box_t l,bounding_box_t p)
{
    
    if(distance(l.x,l.y,p.x,p.y)>l.height+(p.height)*sqrt(2))
    {
        return false;
    }
    
    float vx[] = {p.x+p.width,p.x+p.width,p.x-p.width,p.x-p.width};
    float vy[] = {p.y+p.height,p.y-p.height,p.y+p.height,p.y-p.height};
    int i;
    for(i=0; i<4; i++)
    {
        float x=vx[i],y=vy[i];
        float ly = y;
        float lx = l.x + (y-l.y)/(tan(l.rotation*M_PI/180.0));
        if(abs(lx-x)<=2*l.width)
        {  
            return true;
        }
        if(l.rotation==0 || l.rotation==180)
        {
            if(abs(y-l.y)<=l.width && abs(x-l.x)<=l.height)
            {
                return true;
            }      
        }
        if(l.rotation==90 || l.rotation==270)
        {
            if(abs(x-l.x)<=p.width && abs(y-l.y)>=l.height)
            {
                return true;
            }
        }
    }
    return false;
}


void reset_screen()
{
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
