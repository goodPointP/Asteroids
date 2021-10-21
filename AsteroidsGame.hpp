#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

class GameObject;

class AsteroidsGame {
public:
    AsteroidsGame();
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    void AsteroidsGame::checkCollision(int i);

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);

    sre::Camera camera;
    sre::SDLRenderer r;
    
    bool debugCollisionCircles = false;
    int score = 0;
    int maxScore;
    bool lost = false;
    bool won = false;

};


