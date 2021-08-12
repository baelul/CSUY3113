#include "Menu.h"

void Menu::Initialize(int lives) {
    state.nextScene = -1;
    glClearColor(0, 0, 0, 1.0f);
    
    // Initialize Background
    state.background = new Entity();
    state.background->textureID = Util::LoadTexture("stars.jpg");
    state.background->entityType = BACKGROUND;
    state.background->position = glm::vec3(0, 0, 0);
    state.background->modelMatrix = glm::scale(state.background->modelMatrix, glm::vec3(13, -13, 1));
}

void Menu::Update(float deltaTime) {
    state.background->modelMatrix = glm::rotate(state.background->modelMatrix, glm::radians(5 * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Menu::Render(ShaderProgram *program) {
    state.background->Render(program);
    Util::DrawText(program, fontID, "Moon Walkers", 0.7, -0.05, glm::vec3(-3.5, 0.5, 0));
    Util::DrawText(program, fontID, "Press Enter to Start!", 0.25, -0.05, glm::vec3(-2.2, -0.25, 0));
}
