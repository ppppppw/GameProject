

#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Mesh.h"

struct GameState {
    Entity *player;
    Entity *objects;
    Entity *enemies;
    Entity *door;
    Entity *font;
    
    int nextScene;
    
    
};
class Scene {
public:
    GameState state;
    Mix_Chunk *lose;
    Mix_Chunk *success;
    int Live = 3;
    bool isEnd = false;
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(ShaderProgram *program) = 0;
};
