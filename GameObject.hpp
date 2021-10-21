#pragma once

#include <SDL_events.h>
#include <string>
#include "sre/Sprite.hpp"
#include "sre/SpriteBatch.hpp"

class GameObject {
public:
    explicit GameObject(sre::Sprite sprite);
    virtual ~GameObject();

    virtual void render(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);
    virtual void update(float deltaTime);
    virtual void onKey(SDL_Event& keyEvent);

    bool isForDeletion = false;
    

protected:
    float radius;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    sre::Sprite sprite;

    friend class AsteroidsGame;
};
