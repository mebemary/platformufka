#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct GameBackgroundState : public BaseState
{
    
};

class GameBackgroundGraphicsComponent : public Component<GameBackgroundState>
{
    //sf::RectangleShape rectangle;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    GameBackgroundGraphicsComponent()
    {
        //texture.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\backgroundgame.png");
        texture.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\sky.png");
        sprite.setTexture(texture);
    }

    void update(BaseState &backgroundStateBase, GameState &gameState)
    {
        gameState.render(sprite);
    }
};
