#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;
GLuint heartTextureID;

#define OBJECT_COUNT 133
#define ENEMY_COUNT 3


struct GameState {
    Entity *player;
    Entity *objects;
    Entity *enemies;
    Entity *door;
};

GameState state;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("ZOOMBIE!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    
    
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    fontTextureID = Util::LoadTexture("font1.png");
    heartTextureID = Util::LoadTexture("platformPack_item017.png");
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    //initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-3.5f, 0.75f, 9.5f);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
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
        state.enemies[i].enemyState = 2;
    }
    
    state.enemies[0].position = glm::vec3(0, 0.5f, 5.5f);
    state.enemies[1].position = glm::vec3(7.0f, 0.5f, 5.5f);
    state.enemies[2].position = glm::vec3(2.75f, 0.5f, 4.5f);
  
 
  
    
    state.door = new Entity();
    GLuint doorTextureID = Util::LoadTexture("door.png");
    state.door->billboard = true;
    state.door->textureID = doorTextureID;
    state.door->position = glm::vec3(4.5f, 0.75f, -9.5f);
    state.door->scale = glm::vec3(3, 3, 3);
    state.door->entityType = DOOR;

}


void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                        
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) {
        state.player->rotation.y += 1.0f;
    } else if (keys[SDL_SCANCODE_D]) {
        state.player->rotation.y -= 1.0f;
    }
    
    state.player->velocity.x = 0;
    state.player->velocity.z = 0;
    
    if (keys[SDL_SCANCODE_W]) {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * -2.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * -2.0f;
    } else if (keys[SDL_SCANCODE_S]) {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * 2.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * 2.0f;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        
        state.door->Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        
        for (int i = 0; i < OBJECT_COUNT; i++){
            state.objects[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        }
        
        for (int i = 0; i < ENEMY_COUNT; i++){
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        }
        
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    //state.player->Render(&program);
    
    for (int i = 0; i < OBJECT_COUNT; i++){
        state.objects[i].Render(&program);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(&program);
    }
    
    state.door->Render(&program);
    
    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);
    Util::DrawText(&program, fontTextureID, "Lives: 3", 0.5, -0.3f, glm::vec3(-6, 3.2, 0));
    
    for (int i = 0; i < 3; i++){
        Util::DrawIcon(&program, heartTextureID, glm::vec3(5 + (i * 0.5f), 3.2, 0));
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
