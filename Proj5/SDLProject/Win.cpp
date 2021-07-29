#include "Win.h"

void Win::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0, 0, 0, 1.0f);
}

void Win::Update(float deltaTime) {}

void Win::Render(ShaderProgram *program) {
    Util::DrawText(program, Util::LoadTexture("pixel_font.png"), "You Win!", 0.5, -0.05, glm::vec3(-1.5, 0, 0));
}
