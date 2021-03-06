#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
    movement = glm::vec3(0);
    
    modelMatrix = glm::mat4(1.0f);
    speed = 0.0f;
    billboard = false;
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
}

bool Entity::CheckCollision(Entity *other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Entity *enemies, int enemyCount ,Entity *objects,
        int objectCount)
{
    isCollided = false;
    glm::vec3 previousPosition = position;
   
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    if (entityType == PLAYER) {
        
        for (int i = 0; i < enemyCount; i++)
        {
            if (CheckCollision(&enemies[i])) {
                isCollided = true;
                break;
            }
        }
        
        for (int i = 0; i < objectCount; i++)
        {
            if (objects[i].entityType == FLOOR) continue;
            if (CheckCollision(&objects[i])) {
                position = previousPosition;
                break;
            }
        }
    }
    
    if (entityType == DOOR && CheckCollision(player)) {
        isCollided = true;
    }
    
    if (entityType == ENEMY) {
        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;
            
            //update walking AI
            if (CheckCollision(&objects[i]) && objects[i].entityType == CRATE){
                velocity.z = -velocity.z;
                position += velocity;
            }
        }
    }
    
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Entity::DrawBillboard(ShaderProgram *program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    if (billboard) {
        DrawBillboard(program);
    } else {
        mesh->Render(program);
    }
    
}

