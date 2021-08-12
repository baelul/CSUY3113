#include "Lose.h"

void Lose::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0.094,0.129,0.365, 1.0f);
    
    Mix_HaltMusic();
    
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->movement.y = -1.0f;
    state.player->textureID = Util::LoadTexture("char_and_enemies.png");

    state.player->animDown = new int[3] {106, 106, 106};

    state.player->animIndices = state.player->animDown;
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 17;
    state.player->animRows = 8;
    
    state.player->lives = 3;
    state.player->isActive = true;
}

void Lose::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, NULL, 0, NULL, NULL);
    if (state.player->position.y >= 0) { state.player->movement.y = -1.0f;}
    else if (state.player->position.y <= 0) { state.player->movement.y = 1.0f;}
}

void Lose::Render(ShaderProgram *program) {
    Util::DrawText(program, fontID, "You Died", 0.5, -0.05, glm::vec3(-1.58, 0.75, 0));
    //Util::DrawText(program, fontID, "Press ENTER to try again", 0.25, -0.05, glm::vec3(-2.25, -1, 0));
    state.player->Render(program);
}
