#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"
#include "State.h"

struct MenuState : public BaseState
{

};

class MenuInputComponent : public Component<MenuState>
{
    public:
        void update(BaseState &menuStateBase, GameState &gameState)
        {
            // circle.setPosition(interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor));
            MenuState &menuState = reinterpret_cast<MenuState &>(menuStateBase);

            if (gameState.getInput().yes == KeyState::RELEASED) {
                gameState.pushState<PlayingGameState>();
            }
            else if (gameState.getInput().no == KeyState::RELEASED) {
                gameState.exitGame();
            }
        }
};

class MenuGraphicsComponent : public Component<MenuState>
{
        sf::Font font;
        sf::Text text;
        sf::Texture texture;
        sf::Sprite backgroundSprite;

    public:
        MenuGraphicsComponent()
        {
             if(!font.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\Resources\\SourceCodePro.ttf")) {
	            throw new std::runtime_error("Couldn't load font");
	        }

            texture.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\backgroundmenu.png");
            backgroundSprite.setTexture(texture);
            backgroundSprite.setPosition(0, 0);
	        text.setFont(font);
	        text.setString("Oh hai, press Y to pray game.\nPress N to kill game.");
	        text.setCharacterSize(24);
	        text.setColor(sf::Color::Black);
	        text.setPosition({ 40.0f, 40.0f });
        }

        void update(BaseState &circleStateBase, GameState &gameState)
        {
            gameState.render(backgroundSprite);
            gameState.render(text);
        }
};
