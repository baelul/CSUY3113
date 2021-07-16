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
#define PLATFORM_COUNT 30
#define ENEMY_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

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
    displayWindow = SDL_CreateWindow("Rise of the AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    
    glClearColor(0.878,0.957,0.957, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-2.5, 2.65, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 1.0f;
    state.player->textureID = LoadTexture("mc.png");
    
    state.player->animRight = new int[2] {1, 3};
    state.player->animLeft = new int[2] {0, 2};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 2;
    state.player->animRows = 2;
    
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    
    state.player->jumpPower = 5.0f;
    
    //platform
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint sandTextureID = LoadTexture("sand.png");
    GLuint groundTextureID = LoadTexture("ground.png");
    GLuint stemBTextureID = LoadTexture("stem_bott.png");
    GLuint stemMTextureID = LoadTexture("stem_mid.png");
    GLuint stemTTextureID = LoadTexture("stem_neck.png");
    GLuint mushMTextureID = LoadTexture("mush_mid.png");
    GLuint mushRTextureID = LoadTexture("mush_right.png");
    GLuint mushLTextureID = LoadTexture("mush_left.png");
    
    for (int i = 0; i < 11; i++) {
        state.platforms[i].entityType = PLATFORM;
        if (i < 4) {
            state.platforms[i].textureID = groundTextureID;
            state.platforms[i].position = glm::vec3(-5.25 + i, -3.25f, 0);
        }
        else {
            state.platforms[i].textureID = sandTextureID;
            state.platforms[i].position = glm::vec3(-5.25 + i, -3.25f, 0);
        }
        state.platforms[i].Update(0, NULL, NULL, NULL, 0, 0);
    }
    
    state.platforms[11].textureID = sandTextureID;
    state.platforms[11].position = glm::vec3(-4.25, -2.35, 0);
    
    state.platforms[12].textureID = stemBTextureID;
    state.platforms[12].position = glm::vec3(-4.25, -1.35, 0);
    
    state.platforms[13].textureID = stemMTextureID;
    state.platforms[13].position = glm::vec3(-4.25, -0.35, 0);
    
    state.platforms[14].textureID = stemTTextureID;
    state.platforms[14].position = glm::vec3(-4.25, 0.65, 0);
    
    state.platforms[15].textureID = mushMTextureID;
    state.platforms[15].position = glm::vec3(-4.25, 1.65, 0);
    
    state.platforms[16].textureID = mushMTextureID;
    state.platforms[16].position = glm::vec3(-5.25, 1.65, 0);
    
    state.platforms[17].textureID = mushMTextureID;
    state.platforms[17].position = glm::vec3(-3.25, 1.65, 0);
    
    state.platforms[18].textureID = mushRTextureID;
    state.platforms[18].position = glm::vec3(-2.25, 1.65, 0);
    
    state.platforms[19].textureID = stemBTextureID;
    state.platforms[19].position = glm::vec3(3.25, -2.35, 0);
    
    state.platforms[20].textureID = stemTTextureID;
    state.platforms[20].position = glm::vec3(3.25, -1.35, 0);
    
    state.platforms[21].textureID = mushMTextureID;
    state.platforms[21].position = glm::vec3(3.25, -0.35, 0);
    
    state.platforms[22].textureID = mushMTextureID;
    state.platforms[22].position = glm::vec3(2.25, -0.35, 0);
    
    state.platforms[23].textureID = mushMTextureID;
    state.platforms[23].position = glm::vec3(4.25, -0.35, 0);
    
    state.platforms[24].textureID = mushRTextureID;
    state.platforms[24].position = glm::vec3(5.25, -0.35, 0);
    
    state.platforms[25].textureID = mushMTextureID;
    state.platforms[25].position = glm::vec3(1.25, -0.35, 0);
    
    state.platforms[26].textureID = mushLTextureID;
    state.platforms[26].position = glm::vec3(0.25, -0.35, 0);
    
    state.platforms[27].textureID = sandTextureID;
    state.platforms[27].position = glm::vec3(-5.25, -2.35, 0);
    
    state.platforms[28].textureID = sandTextureID;
    state.platforms[28].position = glm::vec3(-3.25, -2.35, 0);
    
    state.platforms[29].textureID = sandTextureID;
    state.platforms[29].position = glm::vec3(-2.25, -2.35, 0);
    
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].Update(0, NULL, NULL, NULL, 0, 0);
    }

    // enemies
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("enemy.png");
    
    state.enemies[0].position = glm::vec3(4.25, 0.65, 0);
    state.enemies[0].aiType = PATROL;
    state.enemies[0].animIndices = state.player->animLeft;
    
    state.enemies[1].position = glm::vec3(2.25, -2.25, 0);
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].animIndices = state.player->animLeft;
    
    state.enemies[2].position = glm::vec3(-3, -1.25, 0);
    state.enemies[2].aiType = JUMP;
    state.enemies[2].animIndices = state.player->animRight;
    state.enemies[2].jump = true;
    state.enemies[2].jumpPower = 2.0f;
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration = glm::vec3(0, -9.81, 0);
        state.enemies[i].speed = 0.5;
        state.enemies[i].animRight = new int[2] {1, 3};
        state.enemies[i].animLeft = new int[2] {0, 2};
        state.enemies[i].animFrames = 2;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 2;
        state.enemies[i].animRows = 2;
        state.enemies[i].height = 0.8f;
        state.enemies[i].width = 0.8f;
    }
}

bool areEnemiesActive(Entity *enemies) {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (enemies[i].isActive == true) { return true;}
    }
    return false;
}

void ProcessInput() {
    state.player->movement = glm::vec3(0);
    
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
                        
                    case SDLK_SPACE:
                        if (state.player->collidedBottom) {
                            state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(state.player->isActive && areEnemiesActive(state.enemies)) {
        if (keys[SDL_SCANCODE_LEFT]) {
            state.player->movement.x = -1.0f;
            state.player->animIndices = state.player->animLeft;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->movement.x = 1.0f;
            state.player->animIndices = state.player->animRight;
        }
    }
    
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.01666666f
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
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, state.enemies, PLATFORM_COUNT, ENEMY_COUNT);
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, state.enemies, PLATFORM_COUNT, ENEMY_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (state.enemies[i].collidedTop) {
            state.enemies[i].isActive = false;
        }
        state.enemies[i].Render(&program);
    }

    state.player->Render(&program);
    
    //DrawText(&program, LoadTexture("pixel_font.png"), "Mission Success", 0.5, -0.05, glm::vec3(-3.0, 0, 0));

    if(state.player->isActive && areEnemiesActive(state.enemies) == false) {
        DrawText(&program, LoadTexture("pixel_font.png"), "You Win!", 0.5, -0.05, glm::vec3(-1.75, 2, 0));
        
    } else if (state.player->isActive ==  false && areEnemiesActive(state.enemies)) {
        DrawText(&program, LoadTexture("pixel_font.png"), "You Lose...", 0.5, -0.05, glm::vec3(-2, 2, 0));
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
