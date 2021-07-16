#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other) {
    if (other == this) return false;
    if (isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionY(Entity *objects, int objectCount) {
    for(int i = 0; i < objectCount; i++) {
        Entity *object = &objects[i];
        
        if(CheckCollision(object)) {
            object->movement.y = 0;
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                object->collidedBottom = true;
            } else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                object->collidedTop = true;
            }
        }
    }
}

void Entity::CheckCollisionX(Entity *objects, int objectCount) {
    for(int i = 0; i < objectCount; i++) {
        Entity *object = &objects[i];
        
        if(CheckCollision(object)) {
            object->movement.x = 0;
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                object->collidedLeft = true;
            } else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                object->collidedRight = true;
            }
        }
    }
}

void Entity::AIStopping() {
    movement = glm::vec3(0);
}

void Entity::AIWalker() {
    movement = glm::vec3(-1, 0, 0);
}

void Entity::AIWaitAndGo(Entity *player) {
    switch(aiState) {
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = WALKING;
            }
            break;
            
        case WALKING:
            if(player->position.x < position.x) {
                movement = glm::vec3(-1, 0, 0);
                animIndices = animLeft;
            }
            else {
                movement = glm::vec3(1, 0, 0);
                animIndices = animRight;
            }
            break;
            
        case ATTACKING:
            break;
    }
}

void Entity::AIJump() {
    if (velocity.y == 0) {
        velocity.y += jumpPower;
    }
}

void Entity::AIPatrol() { // hard coded, would not do this in the future
    if (position.x < 0.25) {
        movement.x = 1;
        animIndices = animRight;
    } else if (position.x > 4.5) {
        movement.x = -1;
        animIndices = animLeft;
    } else if (position.x > 0.25 && position.x < 4.5) {
        if (animIndices == animRight) {
            movement.x = 1;
        } else { movement.x = -1;}
    }
}


void Entity::AI(Entity *player) {
    switch(aiType) {
        case STOPPING:
            AIStopping();
            break;
            
        case WALKER:
            AIWalker();
            break;
            
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
            
        case PATROL:
            AIPatrol();
            break;
            
        case JUMP:
            AIJump();
            break;
    }
}

bool Entity::areEnemiesActive(Entity *enemies, int enemyCount) {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].isActive == true) { return true;}
    }
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Entity *platforms, Entity *enemies, int platformCount, int enemyCount) {
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedRight = false;
    collidedLeft = false;
    
    if (entityType == ENEMY) {
        AI(player);
    }
    
    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    CheckCollisionY(platforms, platformCount);
    CheckCollisionX(platforms, platformCount);
    
    if (collidedRight || collidedLeft) {
        collidedRight = false;
        collidedLeft = false;
    }
    
    CheckCollisionY(enemies, enemyCount);
    CheckCollisionX(enemies, enemyCount);
    
    for (int i = 0; i < enemyCount; i++) {
        Entity *enemy = &enemies[i];
        // if enemy is hit on the head/jumped on
        if(enemy->collidedTop) {
            enemy->isActive = false;
        }
        
        //if player hits the enemy on the side (r or l)
        if((collidedLeft || collidedRight) || (enemy->collidedLeft || enemy->collidedRight)) {
            if(entityType == PLAYER) {isActive = false;}
            for (int i = 0; i < enemyCount; i++) {
                enemies[i].aiType = STOPPING;
            }
        }
    }
    
    if (areEnemiesActive(enemies, enemyCount) == false || ((collidedLeft || collidedRight) && areEnemiesActive(enemies, enemyCount))) {
        velocity = glm::vec3(0);
        acceleration = glm::vec3(0);
        movement = glm::vec3(0);
    }
  
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false && entityType != PLAYER) return;
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
