#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Asteroid.hpp"
#include "sre/Renderer.hpp"
#include <random>
#include "sre/SpriteAtlas.hpp"
#include <ctime>


std::random_device rd;
std::mt19937 gen(rd());




Asteroid::Asteroid(const sre::Sprite& sprite, int asteroidRank, glm::vec2 previousPosition) : GameObject(sprite) {
    this->asteroidRank = asteroidRank;
    scale = glm::vec2(0.5f, 0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    
    std::uniform_int_distribution<> distr(0, winSize.x);
    std::uniform_int_distribution<> distrA(0, winSize.y);
    std::uniform_int_distribution<> distrPM(1, 2);
    if (asteroidRank == 3) {
        float X = winSize.x + distr(gen);
        float Y = winSize.y + distrA(gen);
        
        position = glm::vec2(X, Y);
        radius = 24;
        radiuss = 24;
    }
    else if (asteroidRank == 2){
        position = previousPosition * 1.01f;
        radius = 11;
        radiuss = 11;
    }
    else {
        position = previousPosition * 1.01f;
        radius = 6;
        radiuss = 6;
    }
    
    velocity = glm::vec2(0.0f, 0.0f);

    std::uniform_int_distribution<> distrRot(1, 360);
    rotation = distr(gen);
}

int Asteroid::returnRank() const {
    return asteroidRank;
}

void Asteroid::update(float deltaTime) {
    existance += deltaTime;
    if (existance > 0.4) {
        justCreated = false;
    }
    
    std::uniform_int_distribution<> distr(1,2);
    if (distr(gen) % 2 == 0) {
        rotation++;
    }
    else {
        rotation--;
    }
    
        float acceleration = deltaTime * thrustPower;
        glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(rotation));
        velocity += direction;
        float speed = glm::length(velocity);
        if (speed > maxSpeed) {
            velocity = velocity * (maxSpeed / speed);
        }
      
    else {
        velocity = velocity * (1.0f - drag * deltaTime);
    }
    position += velocity * deltaTime;


    // wrap around
    if (position.x < 0) {
        position.x += winSize.x;
    }
    else if (position.x > winSize.x) {
        position.x -= winSize.x;
    }
    if (position.y < 0) {
        position.y += winSize.y;
    }
    else if (position.y > winSize.y) {
        position.y -= winSize.y;
    }
}

void Asteroid::checkCollision() {
    
};


