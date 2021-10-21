#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"
#include "sre/SpriteAtlas.hpp"



SpaceShip::SpaceShip(const sre::Sprite &sprite, bool dead, glm::vec2 lastPosition) : GameObject(sprite) {
    this->dead = dead;
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 23;
    radiuss = 23;
    if (dead) {
        position = lastPosition;
    }
    else {
        position = winSize * 0.5f;
    }
    
    velocity = glm::vec2(0.0f,0.0f);
}

void SpaceShip::update(float deltaTime) {
    if (!dead){
        if (thrust){
            float acceleration = deltaTime*thrustPower;
            glm::vec2 direction = glm::rotateZ(glm::vec3(0,acceleration,0), glm::radians(rotation));
            velocity += direction;
            float speed = glm::length(velocity);
            if (speed > maxSpeed){
                velocity = velocity * (maxSpeed / speed);
            }
        } else {
            velocity = velocity * (1.0f - drag*deltaTime);
        }
        position += velocity * deltaTime;
        if (rotateCCW){
            rotation += deltaTime * rotationSpeed;
        }
        if (rotateCW){
            rotation -= deltaTime * rotationSpeed;
        }

        // wrap around
        if (this->position.x < 0){
            position.x += winSize.x;
        } else if (this->position.x > winSize.x){
            position.x -= winSize.x;
        }
        if (this->position.y < 0){
            position.y += winSize.y;
        } else if (this->position.y > winSize.y){
            position.y -= winSize.y;
        }
    }
}

glm::vec2 SpaceShip::getPosition() {
    return glm::vec2(position.x, position.y);
}

//void SpaceShip::onCollision(std::shared_ptr<GameObject> other) {
//    isForDeletion = true;
//}

void SpaceShip::onKey(SDL_Event &keyEvent) {
    if (keyEvent.key.keysym.sym == SDLK_UP){
        thrust = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_LEFT){
        rotateCCW = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_RIGHT){
        rotateCW = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_SPACE) {
        fire = keyEvent.type == SDL_KEYDOWN;
    }
}

void SpaceShip::spawnLaser() {
    
}