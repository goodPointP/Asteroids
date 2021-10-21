#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "Asteroid.hpp"
#include "Laser.hpp"
#include <algorithm>

using namespace sre;

AsteroidsGame::AsteroidsGame() {

    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
        .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    time_t t;
    // random seed based on time
    srand((unsigned)time(&t));

    atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");

    auto spaceshipSprite = atlas->get("playerShip1_blue.png");
    gameObjects.push_back(std::make_shared<SpaceShip>(spaceshipSprite, false, glm::vec2(0,0)));

    auto asteroidSprite = atlas->get("meteorGrey_big1.png");

    

    for (int i = 0; i < 5; i++) {
        gameObjects.push_back(std::make_shared<Asteroid>(asteroidSprite, 3, glm::vec2(0, 0)));
    }

    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event& event) {
        keyEvent(event);
    };

    r.frameRender = [&]() {
        render();
    };

    r.startEventLoop();
}

void AsteroidsGame::update(float deltaTime) {
    if (score >= 35) {
        won = true;
    } else if (!won && !lost) {
        for (int i = 0; i < gameObjects.size(); i++) {
            if (typeid(*gameObjects[i]) == typeid(SpaceShip) || typeid(*gameObjects[i]) == typeid(Laser)) {
                checkCollision(i);
            }
            if (gameObjects[i].get()->isForDeletion) {
                if (typeid(*gameObjects[i]) == typeid(Asteroid) && !std::dynamic_pointer_cast<Asteroid>(gameObjects[i])->justCreated) {
                    gameObjects.erase(gameObjects.begin() + i);
                }
                else {
                    gameObjects.erase(gameObjects.begin() + i);
                }
            }
            else {
                gameObjects[i]->update(deltaTime);
            }
        }
    }
}

void AsteroidsGame::checkCollision(int i) {
    int positionxA = gameObjects[i].get()->position.x;
    int positionyA = gameObjects[i].get()->position.y;
    glm::vec2 positionA = gameObjects[i].get()->position;
    auto& a = *gameObjects[i];
    float radiusA = a.radius;

    for (int j = 0; j < gameObjects.size(); j++) {
        

        glm::vec2 positionB = gameObjects[j].get()->position;

        
        if (typeid(*gameObjects[j]) == typeid(Asteroid)) {
            int positionxB = gameObjects[j].get()->position.x;
            int positionyB = gameObjects[j].get()->position.y;
            auto& b = *gameObjects[j];
            float radiusB = b.radius;

            float distance = glm::distance(positionA, positionB);
            /*std::cout << "Distance to " << typeid(*gameObjects[j]).name() << " is " << distance << std::endl;*/
            
            if (distance <= radiusA + radiusB) {
                //std::cout << radiusA << std::endl;
                //std::cout << radiusB << std::endl;
                //std::cout << "Colliding!"<< std::endl;
                
                //gameObjects[j].get()->isForDeletion = true;

                

                gameObjects[i].get()->isForDeletion = true;
                gameObjects[j].get()->isForDeletion = true;
                
                auto destroyedAsteroid = std::dynamic_pointer_cast<Asteroid>(gameObjects[j]);
                int previousRank = destroyedAsteroid->asteroidRank; // get rank
                glm::vec2 previousPosition = gameObjects[j].get()->position;    // get last known position

                //auto asteroidSprite = atlas->get("meteorBrown_small2.png");
                
                auto asteroidSpriteMedium = atlas->get("meteorBrown_med1.png"); 
                auto asteroidSpriteSmall = atlas->get("meteorBrown_small1.png");

                if (previousRank == 3){
                    for (int i = 0; i < 2; i++) {
                        gameObjects.push_back(std::make_shared<Asteroid>(asteroidSpriteMedium, previousRank - 1, previousPosition));
                    }
                }

                if (previousRank == 2) {
                    for (int i = 0; i < 2; i++) {
                        gameObjects.push_back(std::make_shared<Asteroid>(asteroidSpriteSmall, previousRank - 1, previousPosition));
                    }
                }

                if (typeid(*gameObjects[i]) == typeid(Laser)) {
                    score++;
                }

                if (typeid(*gameObjects[i]) == typeid(SpaceShip)) {
                    auto lastPosition = gameObjects[i].get()->position;
                    auto bangSprite = atlas->get("bang.png");
                    gameObjects.push_back(std::make_shared<SpaceShip>(bangSprite, true, lastPosition));
                    lost = true;
                }
            }
        }
    }
}

void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius){
    int sides = 15;
    float quaterPi = glm::two_pi<float>() / (float)sides;
    for (float f = 0; f<glm::two_pi<float>(); f += quaterPi){
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f)*radius,
                                  position.y + sinf(f)*radius,
                                  0
        });
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f+quaterPi)*radius,
                                  position.y + sinf(f+quaterPi)*radius,
                                  0
        });
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .withClearColor(true, {.20, .60, .86, 1})
            .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->render(spriteBatchBuilder);
        if (typeid(*gameObjects[i]) == typeid(Laser)) {
            auto someLaser = std::dynamic_pointer_cast<Laser>(gameObjects[i]);
            //std::cout << someLaser->timeAlive;
            if (someLaser->timeAlive > 0.35) {
                gameObjects[i].get()->isForDeletion = true;
            };
        }
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles){
        std::vector<glm::vec3> lines;
        for (auto & go : gameObjects){
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr){
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();

    if (won) {
   
    /*ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGui);
    ImGui::Text("You won! Press SPACE to restart.");
    ImGui::End();*/
        ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
        ImGui::Begin("Congratulations!", 0);//ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("    You won!    ");
        ImGui::End();
    }
    if (lost) {

        /*ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x - 100, .0f), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
        ImGui::Begin("", nullptr, ImGui);
        ImGui::Text("You won! Press SPACE to restart.");
        ImGui::End();*/
        ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
        ImGui::Begin("Too bad!", 0);//ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text(" You lost! ");
        ImGui::End();
    }
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->onKey(event);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d){
        debugCollisionCircles = !debugCollisionCircles;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        if (!lost && !won) {
            auto laserSprite = atlas->get("laserBlue01.png");
            auto ship = gameObjects[0].get();

            gameObjects.push_back(std::make_shared<Laser>(laserSprite, ship->position, ship->rotation));

            //std::cout << "X:" << ship->position.x << std::endl;
            //std::cout << "Y:" << ship->position.y << std::endl;
            //std::cout << "........" << std::endl;
        }
        else {

            gameObjects.clear();
            lost = false;
            won = false;
            score = 0;
            atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");

            auto spaceshipSprite = atlas->get("playerShip1_blue.png");
            gameObjects.push_back(std::make_shared<SpaceShip>(spaceshipSprite, false, glm::vec2(0, 0)));

            auto asteroidSprite = atlas->get("meteorGrey_big1.png");

            for (int i = 0; i < 5; i++) {
                gameObjects.push_back(std::make_shared<Asteroid>(asteroidSprite, 3, glm::vec2(0, 0)));
            }

            camera.setWindowCoordinates();

            r.frameUpdate = [&](float deltaTime) {
                update(deltaTime);
            };

            r.keyEvent = [&](SDL_Event& event) {
                keyEvent(event);
            };

            r.frameRender = [&]() {
                render();
            };
        }
    }
}

int main(){
    new AsteroidsGame();
    return 0;
}