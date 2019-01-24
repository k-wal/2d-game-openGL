#include "main.h"
#include "timer.h"
#include "ball.h"
#include "zapper.h"
#include "coin.h"
#include "platform.h"
#include "beam.h"
#include "ring.h"
#include "segment.h"
#include "circle.h"
#include "balloon.h"

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
vector<Beam> beams;
vector<Ring> rings;
vector<Balloon> balloons;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_target_x =0;
float camera_location_x=0;
float camera_rotation_angle = 0;
int count_zappers=0;    //number of zappers currently in range
int count_coins=0;  //number of coins currently in range
int count_beams=1;  //number of beams currently in range
int zappers_hit=0;  //number of zappers hit till now
int beams_hit = 0;  //number of beams hit till now
int score=0;    //score
int coins_scored=0; //number of coins scored till now
int life=0; //number of lives left
int hang=0; //how many ticks left to hang
int life_hang=60;   //how many ticks to hang for when collided by enemy
int space_pressed=0; //if space pressed=1, else 0
float step_length = 0.02;
int zappers_scored = 0;
int beams_scored = 0;
int balloon_wait=0;

Segment digit[5][8]; 

Circle life_circles[10];

int num_digits = 5;

Ring cur_ring;

Timer t60(1.0 / 60);

void draw_score(glm::mat4 VP)
{
    int lines[10][8] = 
    {{0,1,2,3,0,5,6,7},
    {0,0,0,3,0,0,6,0},
    {0,1,0,3,4,5,0,7},
    {0,1,0,3,4,0,6,7},
    {0,0,2,3,4,0,6,0},
    {0,1,2,0,4,0,6,7},
    {0,1,2,0,4,5,6,7},
    {0,1,0,3,0,0,6,0},
    {0,1,2,3,4,5,6,7},
    {0,1,2,3,4,0,6,7}};
    
    int temp = score;
    for(int i=0; i<5; i++)
    {
        for(int j=1; j<=7; j++)
        {
            if(lines[temp%10][j]!=0)
            {
                digit[i][j].draw(VP);

            }
        }
        temp/=10;
    }
}

void draw_life_circles(glm::mat4 VP)
{
    for(int i=0; i<life; i++)
    {
        life_circles[i].draw(VP);
    }
}


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
    platform1.draw(VP);
    //z.draw(VP);
    for(int i=0; i<rings.size(); i++)
    {
        rings[i].draw(VP);
    }
    ball1.draw(VP);
    if(space_pressed)
    {
        ball1.draw_fire(VP);
        space_pressed=0;
    }
    for(int i=0; i<zappers.size(); i++)
    {
        zappers[i].draw(VP);
    }
    for(int i=0; i<coins.size(); i++)
    {
        coins[i].draw(VP);
    }
    for(int i=0; i<beams.size(); i++)
    {
        beams[i].draw(VP);
    }
    for(int i=0; i<balloons.size(); i++)
    {
        balloons[i].draw(VP);
    }
    
    draw_score(VP);
    draw_life_circles(VP);
}

//if number of zappers on the screen is less than one, creates one
void create_zapper()
{
    if(count_zappers>=1)
        return;
    int rnum=rand();
    Zapper z=Zapper(5+camera_location_x,rnum%5-1,rnum%120+30,1.4,COLOR_YELLOW);
    for(int i=0; i<coins.size(); i++)
    {
        if(detect_collision_line(z.bound,coins[i].bound))
        {
            return;
        }
    }
    zappers.push_back(z);
    count_zappers++;
}

void create_beam()
{
    
    if(count_beams>=1)
        return;
    
    int r=rand();
    r%=1000;
    if(r>3)
        return;
    int rnum=rand();
    Beam b = Beam(5+camera_location_x,rnum%5-2,1,COLOR_WHITE,0.07);
    beams.push_back(b);
    count_beams++;
}


//if number of coins on screen are less than 2, creates more coins
void create_coin()
{
    
/*    if(count_coins>=2)
        return;*/
    int r=rand();
    r%=1000;
    if(r>7)
        return;


    int rnum=rand();
    int ny = (rand()%5+2)/2;
    int nx = (rand()%5+2)/2;
    ny*=2;
    nx*=2;
    float x_start = 5+camera_location_x;
    float x_diff = 0.3f;
    float y_start = rnum%4-2;
    float y_diff = 0.3f;
    bounding_box_t b;
    b.x = x_start + (x_diff)*nx/2;
    b.y = y_start + (y_diff)*ny/2;
    b.width = (x_diff+1)*nx;
    b.height = (y_diff+1)*ny;
    Coin c;
    c=Coin(5+camera_location_x,rnum%4-1,COLOR_GOLDEN);


    for(int i=0; i<zappers.size(); i++)
    {
        if(detect_collision_line(zappers[i].bound,b))
            return;
        Zapper z=zappers[i];
    }
    for(int i=0; i<coins.size(); i++)
    {
        if(detect_collision_square(coins[i].bound,b))
            return;
    }

    for(int i=0; i<ny; i++)
    {
        for(int j=0; j<nx; j++)
        {
            c=Coin(x_start+i*x_diff,y_start+j*y_diff,COLOR_GOLDEN);
            coins.push_back(c);
            count_coins++;

        }
    }
}

//counts numbers of each element on the screen and counts zappers hit and coins scored
void count_elements()
{
    count_zappers=zappers.size();
    zappers_hit=0;
    zappers_scored=0;
    for(int i=0; i<zappers.size(); i++)
    {
        Zapper z=zappers[i];
        if(z.position.x<camera_location_x-7)
        {
            count_zappers--;
        }
        if(z.position.x==-100)
        {
            zappers_hit++;
        }
        if(z.position.x==-200)
        {
            zappers_scored++;
        }
    }
    count_coins=coins.size();
    coins_scored=0;
    for(int i=0; i<coins.size(); i++)
    {
        Coin c=coins[i];
        if(c.position.x<camera_location_x-5)
        {
            count_coins--;
        }
        if(c.position.x==-100)
        {
            coins_scored++;
        }
    }
    count_beams=beams.size();
    beams_hit=0;
    beams_scored=0;
    for(int i=0; i<beams.size(); i++)
    {
        Beam b=beams[i];
        if(b.position.x<camera_location_x-9)
        {
            count_beams--;
        }
        if(b.position.x==-100)
        {
            beams_hit++;
        }
        if(b.position.x == -200)
        {
            beams_scored++;
        }
    }
    //printf("%d\n",coins_scored);
}

// -1 : collision at right
// +1 : collision at left
int detect_collision_ring(Ring r,bounding_box_t p)
{
    float rad = (r.r1+r.r2)/2;
    float x=r.position.x,y=r.position.y;
    if(y<=p.y+p.height && y>=p.y-p.height)
    {
        if((x-rad)<=p.x+p.width && (x-rad)>=p.x-p.width)
        {
            return 1;
        }
        if((x+rad)<=p.x+p.width && (x+rad)>=p.x-p.width)
        {
            return -1;
        }
    }
    return 0;
}

// returns true if a balloon collides with z
bool detect_balloon_collision_zapper(bounding_box_t z)
{
    for(int i=0; i<balloons.size(); i++)
    {
        if(balloons[i].is_exist==1)
        {
            if(detect_collision_line(z,balloons[i].bound))
            {
                balloons[i].is_exist=0;
                return true;
            }
        }
    }
    return false;
}

// returns true if a balloon hit b
bool detect_balloon_collision_beam(bounding_box_t b)
{
    for(int i=0; i<balloons.size(); i++)
    {
        if(balloons[i].is_exist==1)
        {
            if(detect_collision_square(b,balloons[i].bound))
            {
                balloons[i].is_exist=0;
                return true;
            }
        }
    }
    return false;
}

//calls other detection functions and removes collided elements from the screen
void detect_all_collisions()
{
    if(ball1.in_ring)
    {
        return;
    }
    for(int i=0; i<zappers.size(); i++)
    {
        if(detect_collision_line(zappers[i].bound,ball1.bound))
        {   
            if(hang==0 && zappers[i].position.x>-100)
                hang = life_hang;
            zappers[i].position.x = -100;
            zappers[i].bound.x = -100;
            
        }
        if(detect_balloon_collision_zapper(zappers[i].bound))
        {
            zappers[i].position.x = -200;
            zappers[i].bound.x = -200;
        }
    }

    for(int i=0; i<beams.size(); i++)
    {
        if(detect_collision_square(beams[i].bound,ball1.bound))
        {
            if(hang==0 && beams[i].position.x!=-100)
                hang = life_hang;
            beams[i].position.x = -100;
            beams[i].update_bounding_box();
            count_beams--;
        }
        if(detect_balloon_collision_beam(beams[i].bound))
        {
            beams[i].position.x = -200;
            beams[i].update_bounding_box();
            count_beams--;
        }

    }
    
    for(int i=0; i<coins.size(); i++)
    {
        if(detect_collision_square(coins[i].bound,ball1.bound))
        {
            coins[i].position.x = -100;
        }
    }

    for(int i=0; i<rings.size(); i++)
    {
        if(detect_collision_ring(rings[i],ball1.bound)==1)
        {
            ball1.position.x = rings[i].position.x - (rings[i].r1+rings[i].r2)/2;
            ball1.in_ring = 1;
            cur_ring = rings[i];
            printf("set at right\n");
        }
        if(detect_collision_ring(rings[i],ball1.bound)==-1)
        {
            ball1.position.x = rings[i].position.x + (rings[i].r1+rings[i].r2)/2;
            ball1.in_ring = 1;
            cur_ring = rings[i];
        }

    }
}

void move_right_in_ring()
{
    if(ball1.in_ring==0)
        return;
    int s;
    if(ball1.position.x<cur_ring.position.x)
        s=1;
    else
        s=-1;
    float x=ball1.position.x,y=ball1.position.y;
    float r=(cur_ring.r1+cur_ring.r2)/2;
    float newy=r*r - (s*(cur_ring.position.x-x)-step_length)*(s*(cur_ring.position.x-x)-step_length);
    newy = sqrt(abs(newy));
    newy = cur_ring.position.y + newy;
    float newx = x+step_length;
    ball1.position.x = newx;
    ball1.position.y = newy;
   
    if(ball1.position.x>=cur_ring.position.x+r)
    {
        //printf("setting free after right\n");
        ball1.position.y-=0.6;
        ball1.in_ring=0;
        ball1.speed_y=0;
        //printf("collision : %d\n",detect_collision_ring(cur_ring,ball1.bound));

    }  
}

void move_left_in_ring()
{
    if(ball1.in_ring==0)
        return;
    int s;
    if(ball1.position.x<cur_ring.position.x)
        s=-1;
    else
        s=1;
    float x=ball1.position.x,y=ball1.position.y;
    float r=(cur_ring.r1+cur_ring.r2)/2;
    float newy=r*r - (s*(cur_ring.position.x-x)-step_length)*(s*(cur_ring.position.x-x)-step_length);
    newy = sqrt(abs(newy));
    newy = cur_ring.position.y + newy;
    float newx = x - step_length;
    ball1.position.x = newx;
    ball1.position.y = newy;
    
    if(ball1.position.x<=cur_ring.position.x-r)
    {
        printf("setting free after left\n");
        ball1.in_ring=0;
        ball1.speed_y=0;
        ball1.position.y-=0.5;
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
    int b = glfwGetKey(window, GLFW_KEY_B);
    //int up  = glfwGetKey(window, GLFW_KEY_UP);
    //int down = glfwGetKey(window, GLFW_KEY_DOWN);
    
    if (left)
    {
        ball1.left_click();
        camera_location_x-=step_length;
        camera_target_x-=step_length;
        detect_all_collisions();

        if(ball1.in_ring)
            move_left_in_ring();

        for(int i=0; i<5; i++)
        {
            for(int j=1; j<=7; j++)
            {
                digit[i][j].position.x-=step_length;
            }
        }
        for(int i=0; i<10; i++)
        {
            life_circles[i].position.x -= step_length;
        }

        
    }
    if(right)
    {
        ball1.right_click();
        camera_location_x+=step_length;
        camera_target_x+=step_length;
        detect_all_collisions();
        if(ball1.in_ring)
            move_right_in_ring();

        for(int i=0; i<5; i++)
        {
            for(int j=1; j<=7; j++)
            {
                digit[i][j].position.x+=step_length;
            }
        }
        for(int i=0; i<10; i++)
        {
            life_circles[i].position.x += step_length;
        }


        create_zapper();
        create_coin();
        create_beam();
    }

    if(b)
    {
        if(balloon_wait==0)
        {
            Balloon bal = Balloon(ball1.position.x+ball1.bound.width,ball1.position.y,COLOR_BLUE);
            balloons.push_back(bal);
            balloon_wait=15;
        }
    }
  
    //    glm::vec3 target (screen_center_x, 0, 0);
    if(space)
    {
        space_pressed=1;
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
    if(balloon_wait>0)
    {
        balloon_wait--;
    }
    char title[1000];
    ball1.tick();
    for(int i=0; i<coins.size(); i++)
    {
        coins[i].tick();
    }
    for(int i=0; i<zappers.size(); i++)
    {
        zappers[i].tick();
    }
    for(int i=0; i<beams.size(); i++)
    {
        beams[i].tick();
    }
    int x=0;
    for(int i=0; i<balloons.size(); i++)
    {
        balloons[i].tick();
        if(balloons[i].is_exist==0)
            x++;
    }
    if(x==balloons.size())
    {
        balloons.clear();
    }
    
    detect_all_collisions();
    count_elements();
    score=coins_scored*5+zappers_scored+beams_scored;
    life=5-zappers_hit-beams_hit;
    sprintf(title,"SCORE : %d\t LIFE : %d",score,life);
    glfwSetWindowTitle(window,title);
    //camera_rotation_angle += 1;
}


void init_life_circles()
{
    float x = -3.5;
    float x_diff = 0.4;
    float y = -3.55;
    float r = 0.1;
    for(int i=0; i<10; i++)
    {
        life_circles[i] = Circle(x,y,COLOR_DARK_RED);
        x += x_diff;
    }
}


void init_segments()
{
    float x = 3.5;
    float x_diff = 0.34;
    float y = -3.6;
    float w = 0.12;


    for(int i=0; i<5; i++)
    {
        digit[i][1] = Segment(x,y+2*w,0,w,COLOR_WHITE);
        digit[i][2] = Segment(x-w,y+w,90,w,COLOR_WHITE);
        digit[i][3] = Segment(x+w,y+w,90,w,COLOR_WHITE);
        digit[i][4] = Segment(x,y,0,w,COLOR_WHITE);
        digit[i][5] = Segment(x-w,y-w,90,w,COLOR_WHITE);
        digit[i][6] = Segment(x+w,y-w,90,w,COLOR_WHITE);
        digit[i][7] = Segment(x,y-w*2,0,w,COLOR_WHITE);
        x-=x_diff;
    }


}


/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1 = Ball(0, 0, COLOR_RED, COLOR_ORANGE);
    Zapper z = Zapper(1,2,30,1.2,COLOR_YELLOW);
    zappers.push_back(z);
    z = Zapper(4,1,60,1.2,COLOR_YELLOW);
    zappers.push_back(z);
/*    Coin c = Coin(1.5,0,COLOR_GOLDEN);
    coins.push_back(c);
    c = Coin(3.5,-2,COLOR_GOLDEN);
    coins.push_back(c);*/
    count_elements();
    Beam beam1 = Beam(7,2,1,COLOR_WHITE,0.07);
    beams.push_back(beam1);

    init_segments();

    init_life_circles();

    Ring ring1 = Ring(10,0,COLOR_GREEN,COLOR_BACKGROUND);
    rings.push_back(ring1);
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

//a : bounding box of square 1
//b : bounding box of square 2
//returns true if collision detected b/w a and b
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
        if(abs(lx-x)<=l.width)
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
