#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"

struct CircleState
{
    sf::Vector2f position { 20.0f, 20.0f };
    sf::Vector2f accelerationVector { 0.0f, 0.0f };
    sf::Vector2f speed { 0.0f, 0.0f };
    sf::Vector2f direction { 0.0f, 0.0f };
};

bool isColliding(CircleState &circle, FloorState &floor)
{
    auto circleLeft = circle.position.x,
        circleRight = circle.position.x + 100.0f,
        circleTop = circle.position.y,
        circleBottom = circle.position.y + 100.0f,
        rectangleLeft = floor.position.x, // - floor.size.x / 2.0f,
        rectangleRight = floor.position.x + floor.size.x, // / 2.0f,
        rectangleTop = floor.position.y, // - floor.size.y / 2.0f,
        rectangleBottom = floor.position.y + floor.size.y; // / 2.0f;

    // std::cout << "C.X=" << "C.Y=" << "C";

    return !(//r2.left > r1.right ||
        circleLeft > rectangleRight ||
        //r2.right < r1.left ||
        circleRight < rectangleLeft ||
        //r2.top > r1.bottom ||
        circleTop > rectangleBottom ||
        //r2.bottom < r1.top);
        circleBottom < rectangleTop
        );
}

class CircleInputComponent : public Component<CircleState>
{
    public:
        void update(CircleState &circleState, GameState &gameState)
        {
            Input &input = gameState.input;

            if (isColliding(circleState, gameState.getFloor()->state) && input.jump == KeyState::PRESSED) {
                std::cout << "MIKI HOP!" << std::endl;
                circleState.speed.y = 400.0f;
            }

            circleState.direction = { (input.left == KeyState::DOWN) * -1.0f + (input.right == KeyState::DOWN) * 1.0f,
                                      (input.jump == KeyState::DOWN) * -1.0f + (input.crouch == KeyState::DOWN) * 1.0f };
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

        void update(CircleState &circleState, GameState &gameState)
        {
            // circle.setPosition(interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor));
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

        void update(CircleState &circleState, GameState &gameState)
        {
            // std::cout << "COLLISION: " << (isColliding(circleState, gameState.getFloor()->state) ? "Y" : "N") << std::endl
            bool isCollidingPrim = isColliding(circleState, gameState.getFloor()->state);

            sf::Vector2f accelerationVector = circleState.direction * acceleration * gameState.tickDelta.asSeconds();

            if (!isCollidingPrim) {
                accelerationVector += sf::Vector2f{ 0.0f, 6.0f };
            }

            sf::Vector2f circleSpeed = circleState.speed;

            if (isCollidingPrim) {
                // std::cout << "COLLISION: " << (isColliding(circleState, gameState.getFloor()->state) ? "Y" : "N") << std::endl;
                circleSpeed.y = -(circleSpeed.y - (circleSpeed.y * 0.05)); // pseudo collision energy loss
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

            if (magnitude > maxSpeed) {
                auto factor = maxSpeed / magnitude;
                circleSpeed.x *= factor;
                circleSpeed.y *= factor;
            }

            circleState.speed = circleSpeed;

            auto deltaPosition = circleSpeed * gameState.tickDelta.asSeconds();

            // currentCirclePosition = nextCirclePosition;
            circleState.position += deltaPosition;
        }
};