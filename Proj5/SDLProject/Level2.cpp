#include "Level2.h"

#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1

unsigned int level2_data[] =
{
    122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    122, 0, 0, 0, 0, 0, 153, 154, 155, 0, 0, 0, 0, 0,
    122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 153, 154, 155,
    122, 0, 0, 0, 153, 154, 155, 0, 0, 0, 0, 0, 0, 0,
    122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    122, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
    122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122
};

void Level2::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0.878,0.957,0.957, 1.0f);
    
    GLuint mapTextureID = Util::LoadTexture("tilemap.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 20, 9);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 1.0f;
    state.player->textureID = Util::LoadTexture("mc.png");
    
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
    
    state.player->lives = lives;
    
    // enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(8, 0, 0);
    state.enemies[0].speed = 0.5;
    
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].animIndices = state.player->animLeft;
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
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

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    }
    
    if (state.player->lives == 0) { state.nextScene = 5;} // lose
    
    bool areEnemiesActive;
        
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive == true) {
            areEnemiesActive = true;
            break;
        } else { areEnemiesActive = false;}
    }
    
    if (state.player->isActive == false) {
        state.player->isActive = true;
        state.player->lives -= 1;
    }
    
    if (state.player->position.x >= 12) { state.nextScene = 3;}
}
void Level2::Render(ShaderProgram *program) {
    Util::DrawText(program, Util::LoadTexture("pixel_font.png"), "Level 2", 0.25, -0.05, glm::vec3(1, -0.5, 0));
    Util::DrawText(program, Util::LoadTexture("pixel_font.png"), "Lives: " + std::to_string(state.player->lives), 0.25, -0.05, glm::vec3(1, -1, 0));
    
    state.map->Render(program);
    state.player->Render(program);
    state.enemies->Render(program);
}
