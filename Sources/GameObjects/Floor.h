#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct FloorState : public BaseState
{
    sf::Vector2f position = { 0.0f, 400.0f };
    sf::Vector2f size = { 800.0f, 200.0f };
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
        //sf::RectangleShape rectangle;
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        FloorGraphicsComponent() //:
            //rectangle({800.0f, 200.0f})
        {
            texture.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\ground.png");
            sprite.setTexture(texture);
            //rectangle.setFillColor(sf::Color::Red);
            //rectangle.setPosition({ 0.0f, 400.0f });
            sprite.setPosition(0.0f, 400.0f);
        }

        void update(BaseState &floorStateBase, GameState &gameState)
        {
            // rectangle.setPosition(rectangleState.position);      
            //gameState.render(rectangle);
            gameState.render(sprite);
        }
};

class FloorPhysicsComponent : public Component<FloorState>
{
    // ain't doin' shit captain
};