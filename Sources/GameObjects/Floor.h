#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct FloorState : public BaseState
{
    sf::Vector2f position = { 80.0f, 400.0f };
    sf::Vector2f size = { 600.0f, 70.0f };
};

class FloorInputComponent : public Component<FloorState>
{
    public:
        void update(FloorState &circleState, GameState &gameState)
        {
            // ain't doin' shit captain
        }
};

class FloorGraphicsComponent : public Component<FloorState>
{
        sf::RectangleShape rectangle;

    public:
        FloorGraphicsComponent() :
            rectangle({600.0f, 70.0f})
        {
            rectangle.setFillColor(sf::Color::Red);
            rectangle.setPosition({ 80.0f, 400.0f });
        }

        void update(BaseState &floorStateBase, GameState &gameState)
        {
            // circle.setPosition(interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor));
            // rectangle.setPosition(rectangleState.position);      
            gameState.renderer->draw(rectangle);
        }
};

class FloorPhysicsComponent : public Component<FloorState>
{
    // ain't doin' shit captain
};