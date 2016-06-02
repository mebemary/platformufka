#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"
#include "Enemy.h"

const float GravityConstant = 6.0f;
const float JumpIntentMargin = 20.0f;
const float JumpSpeed = 700.0f;

struct CircleState : public BaseState
{
    sf::Vector2f position { 20.0f, 20.0f };
    sf::Vector2f accelerationVector { 0.0f, 0.0f };
    sf::Vector2f speed { 0.0f, 0.0f };
    sf::Vector2f direction { 0.0f, 0.0f };
    bool wantJump = false;
    bool didJump = false;
    bool isDed = false;
};

bool isColliding(CircleState &circle, FloorState &floor, float margin = 0.0)
{
    auto circleLeft = circle.position.x,
        circleRight = circle.position.x + 100.0f,
        circleTop = circle.position.y,
        circleBottom = circle.position.y + 100.0f,
        rectangleLeft = floor.position.x - margin,
        rectangleRight = floor.position.x + floor.size.x + margin,
        rectangleTop = floor.position.y - margin,
        rectangleBottom = floor.position.y + floor.size.y + margin;

    return !(circleLeft > rectangleRight ||
             circleRight < rectangleLeft ||
             circleTop > rectangleBottom ||
             circleBottom < rectangleTop);
}

bool isColliding(CircleState &circle, EnemyState &floor, float margin = 0.0)
{
    auto circleLeft = circle.position.x,
        circleRight = circle.position.x + 100.0f,
        circleTop = circle.position.y,
        circleBottom = circle.position.y + 100.0f,
        rectangleLeft = floor.position.x - margin,
        rectangleRight = floor.position.x + floor.size.x + margin,
        rectangleTop = floor.position.y - margin,
        rectangleBottom = floor.position.y + floor.size.y + margin;

    return !(circleLeft > rectangleRight ||
        circleRight < rectangleLeft ||
        circleTop > rectangleBottom ||
        circleBottom < rectangleTop);
}

class CircleInputComponent : public Component<CircleState>
{
    public:
        void update(BaseState &circleStateBase, GameState &gameState)
        {
            CircleState &circleState = reinterpret_cast<CircleState &>(circleStateBase);
            Input &input = gameState.input;

            if (input.no == KeyState::RELEASED || circleState.isDed) {
                gameState.currentState->eventQueue->popState();
                return;
            }

            auto floorList = gameState.getGameObjectsByTag("floor");
            if (!floorList.empty()) {
                auto floor = std::dynamic_pointer_cast<GameObject<FloorState>>(floorList[0]); // std::dynamic_pointer_cast<std::shared_ptr<GameObject<FloorState>>>(floorList[0]);

                if (!circleState.didJump && isColliding(circleState, floor->getState(), JumpIntentMargin) && input.jump == KeyState::DOWN) {
                    circleState.wantJump = true;
                }
                if (circleState.didJump && input.jump == KeyState::RELEASED) {
                    circleState.didJump = false;
                }
            }

            circleState.direction = { (input.left == KeyState::DOWN) * -1.0f + (input.right == KeyState::DOWN) * 1.0f,
                                      0.0f // (input.jump == KeyState::DOWN) * -1.0f + (input.crouch == KeyState::DOWN) * 1.0f
            };
        }
};

class CircleGraphicsComponent : public Component<CircleState>
{
        sf::CircleShape circle;

    public:
        CircleGraphicsComponent() :
            circle(50.0f)
        {
            circle.setFillColor(sf::Color::Magenta);
        }

        void update(BaseState &circleStateBase, GameState &gameState)
        {
            // circle.setPosition(interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor));
            CircleState &circleState = reinterpret_cast<CircleState &>(circleStateBase);
            circle.setPosition(circleState.position);
            gameState.renderer->draw(circle);
        }
};

class CirclePhysicsComponent : public Component<CircleState>
{
        float acceleration = 600.0f;
        float maxSpeed = 400.0f;
        float frictionFactor = 0.01f;
        float movementThreshold = 0.0001f;

    public:
        CirclePhysicsComponent()
        {

        }

        void update(BaseState &circleStateBase, GameState &gameState)
        {
            // std::cout << "COLLISION: " << (isColliding(circleState, gameState.getFloor()->state) ? "Y" : "N") << std::endl
            CircleState &circleState = reinterpret_cast<CircleState &>(circleStateBase);

            auto enemyList = gameState.getGameObjectsByTag("enemy");
            bool isCollidingWithEnemy = false;
            for (auto enemyPtr : enemyList) {
                auto enemy = std::dynamic_pointer_cast<GameObject<EnemyState>>(enemyPtr); // std::dynamic_pointer_cast<std::shared_ptr<GameObject<FloorState>>>(floorList[0]);
                isCollidingWithEnemy = isColliding(circleState, enemy->getState());
            }

            if (isCollidingWithEnemy) {
                circleState.isDed = true;
                return;
            }


            auto floorList = gameState.getGameObjectsByTag("floor");
            bool isCollidingPrim = false;
            if (!floorList.empty()) {
                auto floor = std::dynamic_pointer_cast<GameObject<FloorState>>(floorList[0]); // std::dynamic_pointer_cast<std::shared_ptr<GameObject<FloorState>>>(floorList[0]);
                isCollidingPrim = isColliding(circleState, floor->getState());
            }

            sf::Vector2f accelerationVector = circleState.direction * acceleration * gameState.tickDelta.asSeconds();

            if (!isCollidingPrim) {
                accelerationVector += sf::Vector2f{ 0.0f, GravityConstant };
            }

            sf::Vector2f circleSpeed = circleState.speed;

            if (isCollidingPrim) {
                // std::cout << "COLLISION: " << (isColliding(circleState, gameState.getFloor()->state) ? "Y" : "N") << std::endl;
                circleSpeed.y = -(circleSpeed.y - (circleSpeed.y * 0.05f)); // pseudo collision energy loss
            }

            circleSpeed -= circleSpeed * frictionFactor;

            circleSpeed += accelerationVector;

            if (std::fabs(circleSpeed.x) <= movementThreshold) {
                circleSpeed.x = 0;
            }
            if (std::fabs(circleSpeed.y) <= movementThreshold) {
                circleSpeed.y = 0;
            }

            auto magnitude = std::sqrt(circleSpeed.x * circleSpeed.x + circleSpeed.y * circleSpeed.y);

            /*if (magnitude > maxSpeed) {
                auto factor = maxSpeed / magnitude;
                circleSpeed.x *= factor;
                circleSpeed.y *= factor;
            }*/

            if (circleState.wantJump && isCollidingPrim) {
                std::cout << "MIKI HOP!" << std::endl;
                circleSpeed.y = -JumpSpeed;
                circleState.wantJump = false;
                circleState.didJump = true;
            }

            circleState.speed = circleSpeed;

            auto deltaPosition = circleSpeed * gameState.tickDelta.asSeconds();

            // currentCirclePosition = nextCirclePosition;
            circleState.position += deltaPosition;
        }
};