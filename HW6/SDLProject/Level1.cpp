#include "Level1.h"
#define OBJECT_COUNT 133
#define ENEMY_COUNT 6

GLuint fontTextureID;
GLuint heartTextureID;

glm::mat4 uiViewMatrix, uiProjectionMatrix;


void Level1::Initialize() {
    ohno = Mix_LoadWAV("ohno.mp3");
    
    state.nextScene = -1;
    
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    fontTextureID = Util::LoadTexture("font1.png");
    heartTextureID = Util::LoadTexture("platformPack_item017.png");

    //initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-3.5f, 0.75f, 9.5f);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
    std::cout << state.player->rotation.y;

    state.objects = new Entity[OBJECT_COUNT];

    //initialize platform
    GLint floorTextureID = Util::LoadTexture("floor.jpg");
    Mesh *cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 20);

    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0, -0.25f, 0);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(20, 0.5f, 20);
    state.objects[0].entityType = FLOOR;

    //initialize crate
    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    Mesh *crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);

    int index = 1;
    // left wall
    for (float i = -9.5f; i <= 9.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(9.5, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // right wall
    for (float i = -9.5f; i <= 9.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(-9.5, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // entrance side
    for (float i = -8.5f; i <= 9.5f; i++){
        if (i < -5.5f || i > -2.5f){
            state.objects[index].textureID = crateTextureID;
            state.objects[index].mesh = crateMesh;
            state.objects[index].position = glm::vec3(i, 0.5f, 9.5f);
            state.objects[index].entityType = CRATE;
            index++;
        }
    }

    // exit side
    for (float i = -8.5f; i <= 9.5f; i++){
        if (i < 3.5f || i > 5.5f){
            state.objects[index].textureID = crateTextureID;
            state.objects[index].mesh = crateMesh;
            state.objects[index].position = glm::vec3(i, 0.5f, -9.5f);
            state.objects[index].entityType = CRATE;
            index++;
        }
    }

    // 1v
    for (float i = 8.5f; i >= 5.5f; i--){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(-6.5, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 1h
    for (float i = -5.5f; i <= -2.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(i, 0.5f, 5.5f);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 2v
    for (float i = 8.5f; i >= 2.5f; i--){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(1.5f, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 2h
    for (float i = -5.5f; i < 1.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(i, 0.5f, 2.5f);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 3v
    for (float i = 8.5f; i >= 6.5f; i--){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(5.5f, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    //4h
    for (float i = 6.5f; i < 9.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(i, 0.5f, 2.5f);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 5h
    for (float i = -3.5f; i < 9.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(i, 0.5f, -1.5f);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 6v
    for (float i = -2.5f; i >= -5.5f; i--){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(-6.5, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 7v
    for (float i = -8.5f; i <= -6.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(6.5f, 0.5f, i);
        state.objects[index].entityType = CRATE;
        index++;
    }

    // 6h
    for (float i = -5.5f; i <= 7.5f; i++){
        state.objects[index].textureID = crateTextureID;
        state.objects[index].mesh = crateMesh;
        state.objects[index].position = glm::vec3(i, 0.5f, -5.5f);
        state.objects[index].entityType = CRATE;
        index++;
    }
    std::cout << index;

    //enemy initialization
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("zomb.png");


    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].speed = 1.0f;
        state.enemies[i].width = 1.0f;
        state.enemies[i].height = 1.0f;
        state.enemies[i].depth = 1.0f;
        state.enemies[i].velocity.z = cos(glm::radians(state.enemies[i].rotation.y)) * 0.5f;
    }

    state.enemies[0].position = glm::vec3(0, 0.5f, 5.5f);
    state.enemies[1].position = glm::vec3(7.0f, 0.5f, 7.5f);
    state.enemies[2].position = glm::vec3(2.75f, 0.5f, 6.5f);
    state.enemies[3].position = glm::vec3(-7.55f, 0.5f, 6.5f);
    state.enemies[4].position = glm::vec3(-5.35f, 0.5f, -3.5f);
    state.enemies[5].position = glm::vec3(7.55f, 0.5f, -7.5f);


    //initialization of door
    state.door = new Entity();
    GLuint doorTextureID = Util::LoadTexture("door.png");
    state.door->billboard = true;
    state.door->textureID = doorTextureID;
    state.door->position = glm::vec3(4.5f, 0.75f, -9.5f);
    state.door->scale = glm::vec3(3, 3, 3);
    state.door->entityType = DOOR;

    //initialization of font
    state.font = new Entity();
    state.font->textureID = Util::LoadTexture("font1.png");
    state.font->position = glm::vec3(-1, 1, 0);

    

}
    
void Level1::Update(float deltaTime) {
        state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.objects, OBJECT_COUNT);

        state.door->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.objects, OBJECT_COUNT);

        for (int i = 0; i < OBJECT_COUNT; i++){
            state.objects[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.objects, OBJECT_COUNT);
        }

        for (int i = 0; i < ENEMY_COUNT; i++){
            state.enemies[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.objects, OBJECT_COUNT);
        }
    
}

void Level1::Render(ShaderProgram *program) {
    for (int i = 0; i < OBJECT_COUNT; i++){
        state.objects[i].Render(program);
    }

    for (int i = 0; i < ENEMY_COUNT; i++){
        if (state.enemies[i].isActive == true)
            state.enemies[i].Render(program);
    }

    state.door->Render(program);

    program->SetProjectionMatrix(uiProjectionMatrix);
    program->SetViewMatrix(uiViewMatrix);

    if (state.player->isCollided) {
        Initialize();
        Live--;
    }

    Util::DrawText(program, fontTextureID, "Lives: "+ std::to_string(Live), 0.5, -0.3f, glm::vec3(-6, 3.2, 0));

    for (int i = 0; i < Live; i++){
        Util::DrawIcon(program, heartTextureID, glm::vec3(5 + (i * 0.5f), 3.2, 0));
    }

    if (Live == 0) {
        Util::DrawText(program, state.font->textureID, "You Lose !", 0.5f, -0.05f, glm::vec3(-2,0,0));
        isEnd = true;
    }

    if (state.door->isCollided) {
        Util::DrawText(program, state.font->textureID, "You Win !", 0.5f, -0.05f, glm::vec3(-2,0,0));
        isEnd = true;
    }
    
    
}
