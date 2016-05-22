#pragma once

#include <memory>
#include <map>

#include "Input.h"

class GameState
{
    public:
        Input input;
        sf::Time tickDelta;
        sf::RenderWindow *renderer;
};

template <typename StateT>
class GameObject;

template <typename StateT>
class Component
{
    public:
        virtual void update(StateT &, GameState &) = 0;
};

template <typename StateT>
class GameObject
{
        StateT state;
        std::map<std::string, std::unique_ptr<Component<StateT>>> components;
            
    public:
        virtual void update(GameState &gameState)
        {
            for (auto &pair : components)
            {
                if (pair.first == "graphicsComponent") continue;
                pair.second->update(state, gameState);
            }
        }

        virtual void render(GameState &gameState)
        {
            components.at("graphicsComponent")->update(state, gameState);
        }

        GameObject &addComponent(std::string name, std::unique_ptr<Component<StateT>> component)
        {
            components[name] = std::move(component);
            return *this;
        }
};

struct CircleState
{
    sf::Vector2f position { 20.0f, 20.0f };
    sf::Vector2f accelerationVector { 0.0f, 0.0f };
    sf::Vector2f speed { 0.0f, 0.0f };
    sf::Vector2f direction { 0.0f, 0.0f };
};

class CircleInputComponent : public Component<CircleState>
{
    public:
        void update(CircleState &circleState, GameState &gameState)
        {
            Input &input = gameState.input;

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
            sf::Vector2f accelerationVector = circleState.direction * acceleration * gameState.tickDelta.asSeconds();

            sf::Vector2f circleSpeed = circleState.speed;

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
