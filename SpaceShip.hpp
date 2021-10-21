#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class SpaceShip : public GameObject, public Collidable  {
public:
    SpaceShip(const sre::Sprite& sprite, bool dead, glm::vec2 lastPosition);

    void update(float deltaTime) override;
    //void onCollision(std::shared_ptr<GameObject> other) override;
    void onKey(SDL_Event &keyEvent) override;
    void spawnLaser();
    glm::vec2 getPosition();
    bool fire = false;


    

private:
    bool rotateCW = false;
    bool rotateCCW = false;
    bool thrust = false;
    
    bool dead = false;

    float drag = 0.80f;
    float maxSpeed = 460.0f;
    float thrustPower = 600.0f;
    float rotationSpeed = 100.0f;
    glm::vec2 velocity;
    
    glm::vec2 winSize;
};


