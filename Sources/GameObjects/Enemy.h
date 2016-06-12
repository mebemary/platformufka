#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"

// const float GravityConstant = 6.0f;
// const float JumpIntentMargin = 20.0f;

struct EnemyState : public BaseState
{
    sf::Vector2f position = { 800.0f, 305.0f };
    sf::Vector2f size = { 80.f, 90.0f };
    /*sf::Vector2f accelerationVector { 0.0f, 0.0f };
    sf::Vector2f speed { 0.0f, 0.0f };
    sf::Vector2f direction { 0.0f, 0.0f };*/
};

// bool isColliding(CircleState &circle, FloorState &floor, float margin = 0.0)
// {
//     auto circleLeft = circle.position.x,
//         circleRight = circle.position.x + 100.0f,
//         circleTop = circle.position.y,
//         circleBottom = circle.position.y + 100.0f,
//         rectangleLeft = floor.position.x - margin,
//         rectangleRight = floor.position.x + floor.size.x + margin,
//         rectangleTop = floor.position.y - margin,
//         rectangleBottom = floor.position.y + floor.size.y + margin;

//     return !(circleLeft > rectangleRight ||
//              circleRight < rectangleLeft ||
//              circleTop > rectangleBottom ||
//              circleBottom < rectangleTop);
// }

// class CircleInputComponent : public Component<CircleState>
// {
//     public:
//         void update(BaseState &circleStateBase, GameState &gameState)
//         {
           
//         }
// };

class EnemyGraphicsComponent : public Component<EnemyState>
{
        sf::RectangleShape rectangle;
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        EnemyGraphicsComponent() 
        {
            texture.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\snek.png");
            sprite.setTexture(texture);
        }

        void update(BaseState &enemyBaseState, GameState &gameState)
        {
            EnemyState &enemyState = reinterpret_cast<EnemyState &>(enemyBaseState);
            sprite.setPosition(enemyState.position);
            gameState.render(sprite);
        }
};

class EnemyPhysicsComponent : public Component<EnemyState>
{
    public:
        EnemyPhysicsComponent()
        {

        }

        void update(BaseState &enemyBaseState, GameState &gameState)
        {
            EnemyState &enemyState = reinterpret_cast<EnemyState &>(enemyBaseState);
            enemyState.position.x -= 2.0f;
        }
};

class EnemyAiComponent : public Component < EnemyState >
{
        sf::Vector2f spawnPosition;

    public:
        EnemyAiComponent(sf::Vector2f spawnPosition) :
            spawnPosition(spawnPosition)
        {

        }

        void update(BaseState &enemyBaseState, GameState &gameState)
        {
            // enemyState. { 800.0f, 310.0f }
        }
};