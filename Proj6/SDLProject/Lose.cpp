#include "Lose.h"

void Lose::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0, 0, 0, 1.0f);
}

void Lose::Update(float deltaTime) {}

void Lose::Render(ShaderProgram *program) {
    Util::DrawText(program, Util::LoadTexture("pixel_font.png"), "You Lose", 0.5, -0.05, glm::vec3(-1.5, 0, 0));
}
