#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdlib.h> // import rand
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, p1Matrix, p2Matrix, ballMatrix, gameOverMatrix, projectionMatrix;

glm::vec3 player_1_movement = glm::vec3(0, 0, 0);
glm::vec3 player_1_position = glm::vec3(4.5, 0, 0);

glm::vec3 player_2_movement = glm::vec3(0, 0, 0);
glm::vec3 player_2_position = glm::vec3(-4.5, 0, 0);

glm::vec3 ball_movement = glm::vec3(0, 0, 0);
glm::vec3 ball_position = glm::vec3(0, 0, 0);

float p_speed = 2.5f;
float ball_speed = 1.5f;

float player_height = 1;
float player_width = 0.25;

float ball_height = 0.2;
float ball_width = 0.2;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    p1Matrix = glm::mat4(1.0f);
    p2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    gameOverMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
    player_1_movement = glm::vec3(0);
    player_2_movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // add rand to randomly generate ball direction
                        ball_movement = glm::vec3(1, 1, 0);
                        break;
                }
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_UP]) { player_1_movement.y = 1.0f;}
    else if (keys[SDL_SCANCODE_DOWN]) { player_1_movement.y = -1.0f;}
    
    if (keys[SDL_SCANCODE_W]) { player_2_movement.y = 1.0f;}
    else if (keys[SDL_SCANCODE_S]) { player_2_movement.y = -1.0f;}
    
    if (glm::length(player_1_movement) > 1.0f) { player_1_movement = glm::normalize(player_1_movement);}
    if (glm::length(player_2_movement) > 1.0f) { player_2_movement = glm::normalize(player_2_movement);}
}

bool PaddleCollision() {
    // collision btwn left paddle
    float xdist_1 = fabs(ball_position.x - player_2_position.x) - ((ball_width + player_width) / 2.0f);
    float ydist_1 = fabs(ball_position.y - player_2_position.y) - ((ball_height + player_height) / 2.0f);
    bool c1 = (xdist_1 < 0 && ydist_1 < 0);
    
    // collision btwn right paddle
    float xdist_2 = fabs(player_1_position.x - ball_position.x) - ((player_width + ball_width) / 2.0f);
    float ydist_2 = fabs(player_1_position.y - ball_position.y) - ((ball_height + player_height) / 2.0f);
    bool c2 = (xdist_2 < 0 && ydist_2 < 0);
    
    if (c1 || c2) { return true;}
    return false;
}

float lastTicks = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    // Conditionals to stop players from going off the screen
    if(player_1_position.y > 3) { player_1_position.y = 3;}
    else if(player_1_position.y < -3) { player_1_position.y = -3;}
    
    if(player_2_position.y > 3) { player_2_position.y = 3;}
    else if(player_2_position.y < -3) { player_2_position.y = -3;}

    ballMatrix = glm::mat4(1.0f);
     
    // GAME OVER
    if (ball_position.x > 4.6) {
        p1Matrix = glm::translate(p1Matrix, glm::vec3(0.0f, 0.0f, 0.0f));
        p2Matrix = glm::translate(p2Matrix, glm::vec3(0.0f, 0.0f, 0.0f));
        ballMatrix = glm::translate(ballMatrix, glm::vec3(4.6f, ball_position.y, 0.0f));
    } else if (ball_position.x < -4.6) {
        p1Matrix = glm::translate(p1Matrix, glm::vec3(0.0f, 0.0f, 0.0f));
        p2Matrix = glm::translate(p2Matrix, glm::vec3(0.0f, 0.0f, 0.0f));
        ballMatrix = glm::translate(ballMatrix, glm::vec3(-4.6f, ball_position.y, 0.0f));
    } else {
        // Add (direction * units per second * elapsed time)
        player_1_position += player_1_movement * p_speed * deltaTime;
        player_2_position += player_2_movement * p_speed * deltaTime;
        ball_position += ball_movement * ball_speed * deltaTime;
        
        p1Matrix = glm::mat4(1.0f);
        p1Matrix = glm::translate(p1Matrix, player_1_position);
        
        p2Matrix = glm::mat4(1.0f);
        p2Matrix = glm::translate(p2Matrix, player_2_position);
        
        // bounce ball from top and bottom of screen
        if (ball_position.y > 3.35 || ball_position.y < -3.35) { ball_movement.y *= -1;}
        
        // collision check
        if(PaddleCollision()) { ball_movement.x *= -1;}
        ballMatrix = glm::translate(ballMatrix, ball_position);
        
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(p1Matrix);
    
    float vertices[] = { 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5 };
    
    glVertexAttribPointer(program.positionAttribute, 4, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.5f); // top left
    glVertex2f(0.25f, 0.5f); // top right
    glVertex2f(0.25f, -0.5f); // bottom right
    glVertex2f(-0.0f, -0.5f); // bottom left
    glEnd();
    
    program.SetModelMatrix(p2Matrix);
    glBegin(GL_QUADS);
    glVertex2f(-0.25f, 0.5f); // top left
    glVertex2f(0.0f, 0.5f); // top right
    glVertex2f(0.0f, -0.5f); // bottom right
    glVertex2f(-0.25f, -0.5f); // bottom left
    glEnd();
    
    program.SetModelMatrix(ballMatrix);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, 0.1f); // top left
    glVertex2f(0.1f, 0.1f); // top right
    glVertex2f(0.1f, -0.1f); // bottom right
    glVertex2f(-0.1f, -0.1f); // bottom left
    glEnd();
    
    glDisableVertexAttribArray(program.positionAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
