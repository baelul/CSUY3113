#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
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

void Entity::CheckCollisionsY(Entity *objects, int objectCount) {
    for(int i = 0; i < objectCount; i++) {
        Entity *object = &objects[i];
        
        if(CheckCollision(object)) {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            //if (entityType == PLAYER && objects->entityType == KEY) { objects->isActive = false;}
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

void Entity::CheckCollisionsX(Entity *objects, int objectCount) {
    for(int i = 0; i < objectCount; i++) {
        Entity *object = &objects[i];
        
        if(CheckCollision(object)) {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            //if (entityType == PLAYER && objects->entityType == KEY) { objects->isActive = false;}
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

void Entity::CheckCollisionsY(Map *map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionsX(Map *map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
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
    }
}

bool Entity::areEnemiesActive(Entity *enemies, int enemyCount) {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].isActive == true) { return true;}
    }
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Entity *key, Map *map) {
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

            if (animTime >= 0.1f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames) { animIndex = 0;}
            }
        } else { animIndex = 0;}
    }
    
    velocity.x = movement.x;
    velocity.y = movement.y;
    position += movement * speed * deltaTime;
    
    CheckCollisionsY(map);
    CheckCollisionsX(map);
    
    if (collidedRight || collidedLeft) {
        collidedRight = false;
        collidedLeft = false;
    }
    
    position.y += velocity.y * deltaTime;  // Move on Y
    CheckCollisionsY(objects, objectCount); // Fix if needed
    
    position.x += velocity.x * deltaTime;  // Move on X
    CheckCollisionsX(objects, objectCount); // Fix if needed
    
    for (int i = 0; i < objectCount; i++) {
        Entity *enemy = &objects[i];
        // if enemy is hit on the head/jumped on
        if(enemy->collidedTop) {
            enemy->isActive = false;
        }
        
        //if player hits the enemy on the side (r or l) OR player falls in pit
        if((collidedLeft || collidedRight) || (enemy->collidedLeft || enemy->collidedRight)) {
            if(entityType == PLAYER) {isActive = false;}
            player->position = glm::vec3(2, -2, 0);
        }
    }
    
    // check key collision
    if(CheckCollision(key)) { key->isActive = false;}
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index, int *indices)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    if (indices == animLeft) {
        width *= -1;
    }
    
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
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex], animIndices);
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
