#include "Menu.h"

void Menu::Initialize(int lives) {
    
    state.nextScene = -1;
    glClearColor(0, 0, 0, 1.0f);
    

}

void Menu::Update(float deltaTime) {}

void Menu::Render(ShaderProgram *program) {
    Util::DrawText(program, Util::LoadTexture("pixel_font.png"), "Pixel Platformer", 0.5, -0.05, glm::vec3(-3.35, 1, 0));
    Util::DrawText(program, Util::LoadTexture("pixel_font.png"), "Press Enter to Start!", 0.25, -0.05, glm::vec3(-2.2, 0, 0));
}
