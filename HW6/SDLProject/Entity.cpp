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

//void Entity::AI(Entity *player, float deltaTime){
//    switch(aiType){
//        case WALKER:
//            AIWalker();
//            break;
//            
//        case JUMPER:
//            AIJumper(deltaTime);
//    }
//}
//
//void Entity::AIWalker(){
//    movement = glm::vec3(0, 0, 1);
//}
//
//void Entity::AIJumper(float deltaTime){
//    movement = glm::vec3(0, 0.8, 0);
//    position += movement * speed * deltaTime;
//}
//
//void Entity::AIWaitAndGo(Entity *player){
//    switch(aiState){
//        case WALKING:
//            if (player->position.x < position.x) {
//                movement = glm::vec3(-1,0,0);
//            }else {
//                movement = glm::vec3(1,0,0);
//            }
//            break;
//            
//        case JUMPING:
//            break;
//    }
//}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount)
{
    glm::vec3 previousPosition = position;
    
    if (billboard && entityType == ENEMY) {
//        float directionX = position.x - player->position.x;
//        float directionZ = position.z - player->position.z;
//        rotation.y = glm::degrees(atan2f(directionX, directionZ));
        //AI(player, deltaTime);
        //velocity.z = cos(glm::radians(rotation.y)) * 1.0f;

    }
   
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
            
            
            switch (enemyState) {
                case 1:
                    velocity.z = cos(glm::radians(rotation.y)) * 1.0f;
                    if (CheckCollision(&objects[i]) && objects[i].entityType == CRATE){
                        std::cout << 0;
                        enemyState = 2;
                    }
                    break;
                case 2:
                    velocity.z = cos(glm::radians(rotation.y)) * -1.0f;
                    if (CheckCollision(&objects[i]) && objects[i].entityType == CRATE){
                        enemyState = 1;
                        std::cout << 1;
                    }
                    break;
                default:
                    break;
            }
           
            
            
            
//            if (CheckCollision(&objects[i]) && objects[i].entityType == CRATE) {
//                velocity.z = cos(glm::radians(rotation.y)) * -1.0f;
//                break;
//            }
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

