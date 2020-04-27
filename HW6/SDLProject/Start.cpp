#include "Start.h"


void Start::Initialize() {
    state.nextScene = -2;
    //initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-3.5f, 0.75f, 9.5f);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
    state.font = new Entity();
    state.font->textureID = Util::LoadTexture("font1.png");
    state.font->position = glm::vec3(1,-1,0);
    

}
    
void Start::Update(float deltaTime) {
}


void Start::Render(ShaderProgram *program) {

    Util::DrawText(program, state.font->textureID, "ZOOMBIE", 0.6f, -0.05f, glm::vec3(-5,1,0));
    Util::DrawText(program, state.font->textureID, "PRESS ENTER TO START", 0.25f, -0.05f, glm::vec3(-5.25,0,0));
    
    
}
