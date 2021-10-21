#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Laser : public GameObject, public Collidable {
public:
    Laser(const sre::Sprite &sprite, glm::vec2 shipPosition, int shipRotation);
    ~Laser();
    void update(float deltaTime) override;
    //void onCollision(std::shared_ptr<GameObject> other) override;
    void onKey(SDL_Event &keyEvent) override;
    bool isOffScreen = false;
    float timeAlive = 0;
    std::vector<std::shared_ptr<GameObject>> gameObjects;

private:
    bool rotateCW = false;
    bool rotateCCW = false;
    bool thrust = true;
    
    
    float drag = 0.0f;
    float maxSpeed = 9999.0f;
    float thrustPower = 6000.0f;
    float rotationSpeed = 100.0f;
    glm::vec2 velocity;

    glm::vec2 winSize;
};

