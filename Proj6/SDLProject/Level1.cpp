#include "Level1.h"

#define LEVEL1_WIDTH 16
#define LEVEL1_HEIGHT 16

#define LEVEL1_ENEMY_COUNT 0

unsigned int level1_data[] =
{
    0, 152, 0, 0, 0, 13, 2, 1, 2, 1, 3, 0, 169, 0, 152, 0,
    0, 0, 0, 0, 152, 52, 27, 27, 27, 27, 55, 152, 0, 144, 152, 0,
    0, 169, 144, 0, 0, 26, 0, 0, 0, 27, 29, 0, 0, 144, 0, 0,
    0, 144, 0, 152, 0, 52, 0, 0, 0, 33, 55, 169, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 26, 0, 0, 0, 0, 29, 0, 0, 152, 0, 0,
    13, 1, 2, 1, 3, 52, 0, 0, 0, 0, 55, 13, 2, 1, 2, 16,
    26, 27, 27, 27, 29, 26, 0, 0, 0, 0, 29, 52, 11, 27, 27, 55,
    52, 27, 6, 27, 31, 30, 0, 0, 0, 0, 55, 26, 32, 33, 33, 29,
    26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 78, 79, 80, 79, 81,
    78, 79, 80, 79, 80, 83, 0, 0, 0, 0, 55, 0, 0, 0, 0, 0,
    0, 0, 0, 144, 0, 52, 0, 0, 0, 0, 55, 0, 144, 0, 0, 144,
    0, 152, 169, 0, 0, 26, 0, 0, 0, 0, 29, 0, 0, 0, 144, 0,
    0, 0, 0, 0, 0, 52, 0, 0, 0, 0, 55, 0, 144, 169, 0, 0,
    0, 144, 0, 0, 152, 26, 0, 0, 0, 0, 29, 152, 0, 0, 0, 0,
    0, 169, 0, 144, 0, 52, 0, 0, 0, 0, 55, 0, 0, 0, 144, 0,
    0, 152, 0, 0, 0, 78, 79, 80, 79, 80, 81, 0, 152, 0, 0, 0
};

unsigned int level1_floor_and_details[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 11, 0, 115, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 27, 10, 56, 36, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 35, 33, 33, 62, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 61, 54, 54, 53, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 61, 54, 53, 107, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 60, 54, 54, 54, 0, 0, 0, 36, 36, 0,
    0, 0, 0, 0, 0, 0, 61, 54, 54, 54, 0, 0, 0, 62, 62, 0,
    0, 33, 32, 33, 33, 33, 34, 54, 54, 53, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 107, 84, 54, 54, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 53, 54, 54, 54, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 54, 54, 54, 84, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 54, 54, 54, 54, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 54, 53, 54, 54, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 54, 107, 54, 53, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void Level1::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0.094,0.129,0.365, 1.0f);
    
    // Halt and Initialize Music
    Mix_HaltMusic();
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_PlayMusic(Mix_LoadMUS("find-the-exit.wav"), -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    
    // Initialize Map
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 26, 8);
    
    // Initialize Floor and Details
    state.floor_and_details = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_floor_and_details, mapTextureID, 1.0f, 26, 8);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(7.5, -12.5, 0);
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
    
    state.player->lives = 3;
    
    // Initialize Enemies
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    
    // Initialize Key
    state.key = new Entity();
    state.key->entityType = KEY;
    state.key->position = glm::vec3(1, -8, 0);
    state.key->textureID = Util::LoadTexture("key.png");
    state.key->modelMatrix = glm::scale(state.key->modelMatrix, glm::vec3(0.75, 0.75, 1));
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.key, state.map);
    state.key->Update(1.0f, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.key, state.map);
    
    if (state.player->lives == 0) {
        Mix_PlayChannel(-1, lose, 0);
        Mix_Volume(-1,MIX_MAX_VOLUME/8);
        state.nextScene = 5;
    } // lose
    
    if (state.player->isActive == false) {
        state.player->isActive = true;
        state.player->lives -= 1;
    }
    
    bool within_x_range = state.player->position.x >= 6 && state.player->position.x <= 8;
    bool within_y_range = state.player->position.y >= -2 && state.player->position.y <= 0;
    
    if (state.key->isActive == false && within_x_range && within_y_range)
    { state.nextScene = 2;} // CHANGE TO POSITION OF STAIRS && CHECK FOR KEY
}

void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.floor_and_details->Render(program);
    state.key->Render(program);
    state.player->Render(program);
    Util::DrawText(program, fontID, "Level 1", 0.25, -0.05, glm::vec3(state.player->position.x - 4, state.player->position.y + 3, 0));
    Util::DrawText(program, fontID, "Lives: " + std::to_string(state.player->lives), 0.25, -0.05, glm::vec3(state.player->position.x - 4, state.player->position.y + 2.6, 0));
}
