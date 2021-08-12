#include "Win.h"

void Win::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0.094,0.129,0.365, 1.0f);
    
    Mix_HaltMusic();
    
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    //state.player->modelMatrix = glm::scale(state.player->modelMatrix, glm::vec3(5, 5, 0));
    state.player->textureID = Util::LoadTexture("char_and_enemies.png");

    state.player->animDown = new int[8] {18, 19, 20, 21, 22, 23, 24, 25};

    state.player->animIndices = state.player->animDown;
    state.player->animFrames = 7;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 17;
    state.player->animRows = 8;
    
    state.player->lives = 3;
}

void Win::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, NULL, 0, NULL, NULL);
    if (state.player->position.y >= 0) { state.player->movement.y = -1.0f;}
    else if (state.player->position.y <= 0) { state.player->movement.y = 1.0f;}
}

void Win::Render(ShaderProgram *program) {
    Util::DrawText(program, fontID, "You Win!", 0.5, -0.05, glm::vec3(-1.58, 1, 0));
    Util::DrawText(program, fontID, "Press ENTER to play again!", 0.25, -0.05, glm::vec3(-2.5, -1, 0));
    state.player->Render(program);
}
