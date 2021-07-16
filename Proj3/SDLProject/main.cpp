#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "vector"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#define PLATFORM_COUNT 31

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *landing;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint fontTextureId;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

void DrawText(ShaderProgram *program, GLuint fontTextureId, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureId);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    fontTextureId = LoadTexture("font1.png");
    
    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0, 3, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration.y = -0.25;
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("playerShip2_blue.png");
    
    state.player->height = 0.8f;
    state.player->width = 0.7f;
    
    //Initialize Platforms
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureId = LoadTexture("platformPack_tile007.png");
    
    // floor
    state.platforms[0].textureID = platformTextureId;
    state.platforms[0].position = glm::vec3(-3.5, -3.25f, 0);
    
    state.platforms[1].textureID = platformTextureId;
    state.platforms[1].position = glm::vec3(-2.5, -3.25f, 0);
    
    state.platforms[2].textureID = platformTextureId;
    state.platforms[2].position = glm::vec3(0.5, -3.25f, 0);
    
    state.platforms[3].textureID = platformTextureId;
    state.platforms[3].position = glm::vec3(1.5, -3.25f, 0);
    
    state.platforms[4].textureID = platformTextureId;
    state.platforms[4].position = glm::vec3(2.5, -3.25f, 0);
    
    state.platforms[5].textureID = platformTextureId;
    state.platforms[5].position = glm::vec3(3.5, -3.25f, 0);
    
    //left side
    state.platforms[6].textureID = platformTextureId;
    state.platforms[6].position = glm::vec3(-4.5, -3.25f, 0);
    
    state.platforms[7].textureID = platformTextureId;
    state.platforms[7].position = glm::vec3(-4.5, -2.25f, 0);
    
    state.platforms[8].textureID = platformTextureId;
    state.platforms[8].position = glm::vec3(-4.5, -1.25f, 0);
    
    state.platforms[9].textureID = platformTextureId;
    state.platforms[9].position = glm::vec3(-4.5, -0.25f, 0);
    
    state.platforms[10].textureID = platformTextureId;
    state.platforms[10].position = glm::vec3(-4.5, 0.75f, 0);
    
    state.platforms[11].textureID = platformTextureId;
    state.platforms[11].position = glm::vec3(-4.5, 1.75f, 0);
    
    state.platforms[12].textureID = platformTextureId;
    state.platforms[12].position = glm::vec3(-4.5, 2.75f, 0);
    
    state.platforms[13].textureID = platformTextureId;
    state.platforms[13].position = glm::vec3(-4.5, 3.75f, 0);
    
    //right side
    state.platforms[14].textureID = platformTextureId;
    state.platforms[14].position = glm::vec3(4.5, -3.25f, 0);
    
    state.platforms[15].textureID = platformTextureId;
    state.platforms[15].position = glm::vec3(4.5, -2.25f, 0);
    
    state.platforms[16].textureID = platformTextureId;
    state.platforms[16].position = glm::vec3(4.5, -1.25f, 0);
    
    state.platforms[17].textureID = platformTextureId;
    state.platforms[17].position = glm::vec3(4.5, -0.25f, 0);
    
    state.platforms[18].textureID = platformTextureId;
    state.platforms[18].position = glm::vec3(4.5, 0.75f, 0);
    
    state.platforms[19].textureID = platformTextureId;
    state.platforms[19].position = glm::vec3(4.5, 1.75f, 0);
    
    state.platforms[20].textureID = platformTextureId;
    state.platforms[20].position = glm::vec3(4.5, 2.75f, 0);
    
    state.platforms[21].textureID = platformTextureId;
    state.platforms[21].position = glm::vec3(4.5, 3.75f, 0);
    
    // extend left
    state.platforms[22].textureID = platformTextureId;
    state.platforms[22].position = glm::vec3(-4.5, 1.75f, 0);
    
    state.platforms[23].textureID = platformTextureId;
    state.platforms[23].position = glm::vec3(-3.5, 1.75f, 0);
    
    state.platforms[24].textureID = platformTextureId;
    state.platforms[24].position = glm::vec3(-2.5, 1.75f, 0);
    
    state.platforms[25].textureID = platformTextureId;
    state.platforms[25].position = glm::vec3(-1.5, 1.75f, 0);
    
    // extend right
    state.platforms[26].textureID = platformTextureId;
    state.platforms[26].position = glm::vec3(4.5, -0.25f, 0);
    
    state.platforms[27].textureID = platformTextureId;
    state.platforms[27].position = glm::vec3(3.5, -0.25f, 0);
    
    state.platforms[28].textureID = platformTextureId;
    state.platforms[28].position = glm::vec3(2.5, -0.25f, 0);
    
    state.platforms[29].textureID = platformTextureId;
    state.platforms[29].position = glm::vec3(1.5, -0.25f, 0);
    
    state.platforms[30].textureID = platformTextureId;
    state.platforms[30].position = glm::vec3(0.5, -0.25f, 0);
    
    
    //Initialize landing
    state.landing = new Entity[2];
    GLuint landingTextureId = LoadTexture("platformPack_tile008.png");
    
    state.landing[0].textureID = landingTextureId;
    state.landing[0].position = glm::vec3(-1.5, -3.25f, 0);
    
    state.landing[1].textureID = landingTextureId;
    state.landing[1].position = glm::vec3(-0.5, -3.25f, 0);
    
    
    //Update
    for(int i = 0; i< PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, state.platforms, state.landing, 0);
    }
    
    for(int i = 0; i< 2; i++) {
        state.landing[i].Update(0, state.platforms, state.landing, 0);
    }
    
    //state.message->Update(0, state.platforms, state.landing, 0);
}

void ProcessInput() {
    
    state.player->acceleration.x = 0;
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->acceleration.x = -0.25f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->acceleration.x = 0.25f;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP, not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.platforms, state.landing, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    
    for (int i = 0; i < 2; i++) {
        state.landing[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    DrawText(&program, fontTextureId, "Mission Success", 0.75, -0.25f, glm::vec3(-3.375, 0.75, 0));
    
    if(state.landing->collidedTop) {
        DrawText(&program, fontTextureId, "Mission Success", 0.75, -0.25f, glm::vec3(-3.375, 0.75, 0));
    } else if (state.platforms->collidedTop || state.platforms->collidedRight || state.platforms->collidedLeft) {
        DrawText(&program, fontTextureId, "Mission Failed", 0.75, -0.25f, glm::vec3(-3.375, 0.75, 0));
    }
    
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
