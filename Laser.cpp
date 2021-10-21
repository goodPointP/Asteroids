#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Laser.hpp"
#include "sre/Renderer.hpp"
#include "sre/SpriteAtlas.hpp"


Laser::Laser(const sre::Sprite &sprite, glm::vec2 shipPosition, int shipRotation) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 12;
    radiuss = 12;
    rotation = shipRotation;
    position = glm::vec2(shipPosition.x+10, shipPosition.y + 10);
    velocity = glm::vec2(0.0f,0.0f);
}

void Laser::update(float deltaTime) {
    timeAlive += deltaTime;
        float acceleration = deltaTime*thrustPower;
        glm::vec2 direction = glm::rotateZ(glm::vec3(0,acceleration,0), glm::radians(rotation));
        velocity += direction;
        float speed = glm::length(velocity);
        if (speed > maxSpeed){
            velocity = velocity * (maxSpeed / speed);
        }
    position += velocity * deltaTime;

  /*  std::cout << "X:" << position.x << std::endl;
    std::cout << "Y:" << position.y << std::endl;
    std::cout << "........" << std::endl;*/

    // wrap around
    if (position.x < 0){
        isForDeletion = true;
    } else if (position.x > winSize.x){
        isForDeletion = true;
    }
    if (position.y < 0){
        isForDeletion = true;
    } else if (position.y > winSize.y){
        /*delete &gameObjects;*/
        isForDeletion = true;
    }
    if (isOffScreen) {
        return;
    }
}

Laser::~Laser() {};



void Laser::onKey(SDL_Event &keyEvent) {
    if (keyEvent.key.keysym.sym == SDLK_UP){
        thrust = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_LEFT){
        rotateCCW = keyEvent.type == SDL_KEYDOWN;
    }
    if (keyEvent.key.keysym.sym == SDLK_RIGHT){
        rotateCW = keyEvent.type == SDL_KEYDOWN;
    }
}
