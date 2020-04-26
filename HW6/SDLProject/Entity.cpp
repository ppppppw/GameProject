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
//    isCollided = false;
}

bool Entity::CheckCollision(Entity *other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount)
{
//    isCollided = false;
    glm::vec3 previousPosition = position;
   
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    if (entityType == PLAYER) {
        for (int i = 0; i < objectCount; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;
            
            if (CheckCollision(&objects[i]) && objects[i].entityType == CRATE) {
                position = previousPosition;
                
                break;
            }

        }
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
            
            if (CheckCollision(player)) {
//                player->position = glm::vec3(-3.5f, 0.75f, 9.5f);
                isCollided = true;
                player->live--;
                std::cout << 1;
                isActive = false;
                position = glm::vec3(-3.5f, 0.75f, 9.5f);
                break;
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

