#include "Level3.h"

#define LEVEL3_WIDTH 17
#define LEVEL3_HEIGHT 19

#define LEVEL3_ENEMY_COUNT 2

unsigned int level3_data[] =
{
    13, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 16,
    26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 29,
    52, 0, 0, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 55,
    26, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 33, 33, 29,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 55,
    26, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 5, 29,
    52, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 55,
    26, 0, 0, 0, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 5, 29,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 55,
    26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29,
    78, 79, 80, 79, 80, 83, 0, 0, 0, 82, 80, 83, 0, 0, 0, 0, 55,
    0, 0, 0, 0, 0, 26, 0, 0, 133, 55, 0, 78, 79, 83, 0, 0, 29,
    13, 1, 2, 1, 2, 30, 0, 0, 0, 31, 1, 2, 16, 52, 0, 0, 55,
    26, 27, 27, 27, 27, 27, 0, 0, 0, 27, 27, 27, 29, 26, 0, 0, 29,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 33, 55, 52, 0, 0, 55,
    26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 26, 0, 0, 29,
    78, 79, 83, 0, 82, 80, 79, 80, 79, 80, 79, 80, 81, 26, 0, 0, 55,
    0, 0, 78, 79, 81, 0, 0, 0, 0, 0, 0, 0, 0, 52, 0, 0, 29,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 78, 79, 80, 81
};

unsigned int level3_floor_and_details[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 7, 0,
    0, 27, 10, 38, 0, 57, 0, 0, 57, 0, 0, 57, 0, 38, 38, 38, 0,
    0, 35, 33, 64, 33, 33, 33, 33, 33, 33, 33, 33, 33, 64, 64, 64, 0,
    0, 60, 53, 54, 54, 54, 84, 54, 84, 54, 54, 107, 54, 5, 5, 36, 0,
    0, 60, 54, 86, 54, 4, 4, 4, 4, 4, 54, 54, 54, 5, 5, 62, 0,
    0, 61, 54, 54, 54, 4, 4, 4, 4, 4, 54, 84, 54, 5, 5, 5, 0,
    0, 60, 54, 53, 54, 4, 4, 4, 4, 4, 86, 53, 54, 5, 5, 36, 0,
    0, 61, 85, 54, 54, 54, 84, 54, 54, 54, 54, 54, 54, 5, 5, 62, 0,
    0, 60, 54, 54, 86, 54, 54, 54, 84, 54, 54, 84, 54, 5, 5, 5, 0,
    0, 0, 0, 0, 0, 0, 87, 54, 54, 0, 0, 0, 54, 54, 54, 54, 0,
    0, 0, 169, 144, 0, 0, 133, 54, 0, 0, 0, 0, 0, 0, 54, 53, 0,
    0, 0, 0, 0, 0, 0, 61, 54, 54, 0, 0, 0, 0, 0, 85, 54, 0,
    0, 0, 28, 6, 28, 0, 60, 54, 54, 0, 38, 38, 0, 0, 84, 54, 0,
    0, 33, 33, 32, 33, 33, 34, 54, 54, 58, 64, 64, 0, 0, 53, 54, 0,
    0, 54, 84, 54, 54, 54, 63, 54, 84, 54, 54, 54, 0, 0, 54, 86, 0,
    0, 0, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0,
    152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 152, 0, 169, 0, 4, 4, 0,
    0, 152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 144, 0, 0, 0, 0, 0
};

void Level3::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0.094,0.129,0.365, 1.0f);
    
    // Halt and Initialize Music
    Mix_HaltMusic();
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayMusic(Mix_LoadMUS("galactic-trek.wav"), -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    
    // Initialize Map
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 26, 8);
    
    // Initialize Floor and Details
    state.floor_and_details = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_floor_and_details, mapTextureID, 1.0f, 26, 8);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3, -15.5, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 5.0f;
    state.player->textureID = Util::LoadTexture("char_and_enemies.png");
    
    state.player->animRight = new int[6] {70, 71, 72, 73, 74, 75};
    state.player->animLeft = new int[6] {70, 71, 72, 73, 74, 75};
    state.player->animUp = new int[6] {87, 88, 89, 90, 91, 92};
    state.player->animDown = new int[6] {53, 54, 55, 56, 57, 58};

    state.player->animIndices = state.player->animUp;
    state.player->animFrames = 5;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 17;
    state.player->animRows = 8;
    
    state.player->width = 0.8f;
    
    state.player->lives = lives;
    
    // Initialize Enemies
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("char_and_enemies.png");
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].speed = 3;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].animRight = new int[4] {45, 46, 47, 48};
        state.enemies[i].animLeft = new int[4] {45, 46, 47, 48};
        state.enemies[i].animUp = new int[4] {45, 46, 47, 48};
        state.enemies[i].animDown = new int[4] {45, 46, 47, 48};
        state.enemies[i].animFrames = 3;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 17;
        state.enemies[i].animRows = 8;
        state.enemies[i].height = 0.8f;
        state.enemies[i].width = 0.8f;
    }
    
    state.enemies[0].position = glm::vec3(7, -8.5, 0); // range x: 3-11
    state.enemies[0].movement.x = -1;
    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[0].aiType = PATROL_LVL3_1;
    
    state.enemies[1].position = glm::vec3(14, -8.5, 0); // range y: -4.5 - -14.5
    state.enemies[1].animIndices = state.enemies[1].animRight;
    state.enemies[1].movement.y = -1;
    state.enemies[1].aiType = PATROL_LVL3_2;
    
    // Initialize Key
    state.key = new Entity();
    state.key->entityType = KEY;
    state.key->position = glm::vec3(14.5, -16.5, 0);
    state.key->textureID = Util::LoadTexture("key.png");
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.key, state.map);
    state.key->Update(1.0f, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.key, state.map);
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.key, state.map);
    }
    
    if (state.player->lives == 0) {
        Mix_PlayChannel(-1, lose, 0);
        Mix_Volume(-1,MIX_MAX_VOLUME/8);
        state.nextScene = 5;
    } // lose
    
    if (state.player->isActive == false) {
        state.player->isActive = true;
        state.player->lives -= 1;
        state.player->position = glm::vec3(3, -15.5, 0);
    }
    
    bool within_x_range = state.player->position.x >= 1 && state.player->position.x <= 2.5;
    bool within_y_range = state.player->position.y >= -2 && state.player->position.y <= -0.5;

    if (state.key->isActive == false && within_x_range && within_y_range)
    {
        Mix_PlayChannel(-1, win, 0);
        Mix_Volume(-1,MIX_MAX_VOLUME/2);
        state.nextScene = 4;
    } // win
    
}
void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.floor_and_details->Render(program);
    state.key->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    state.player->Render(program);
    Util::DrawText(program, fontID, "Level 3", 0.25, -0.05, glm::vec3(state.player->position.x - 4, state.player->position.y + 3, 0));
    Util::DrawText(program, fontID, "Lives: " + std::to_string(state.player->lives), 0.25, -0.05, glm::vec3(state.player->position.x - 4, state.player->position.y + 2.6, 0));
}
