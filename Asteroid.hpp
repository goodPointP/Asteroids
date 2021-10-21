#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"
#include <stdlib.h> 
#include <stdio.h>    
#include <random>
#include <ctime>
#include <time.h>



class Asteroid : public GameObject, public Collidable {
public:
    Asteroid(const sre::Sprite& sprite, int asteroidRank, glm::vec2 previousPosition);

    void update(float deltaTime) override;
    //void onCollision(std::shared_ptr<GameObject> other) override;
    void checkCollision();
    int returnRank() const;
    int asteroidRank;
    bool justCreated = true;

private:
    bool rotateCW = false;
    bool rotateCCW = false;
    bool thrust = false;
    bool fire = false;
    float existance = 0;
    

    


    float drag = 0.80f;
    float maxSpeed = 20.0f;
    float thrustPower = 20.0f;
    float rotationSpeed = 100.0f;
    glm::vec2 velocity;

    glm::vec2 winSize;
};


