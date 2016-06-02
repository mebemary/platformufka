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

            auto eventQueue = gameState.currentState->eventQueue;

            if (gameState.input.yes == KeyState::RELEASED) {
                eventQueue->pushState(std::make_shared<PlayingGameState>(gameState.tickDelta, eventQueue));
            }
            else if (gameState.input.no == KeyState::RELEASED) {
                eventQueue->exitGame();
            }
        }
};

class MenuGraphicsComponent : public Component<MenuState>
{
        sf::Font font;
        sf::Text text;

    public:
        MenuGraphicsComponent()
        {
             if(!font.loadFromFile("C:\\Users\\Andrzej\\Documents\\platformufka\\Resources\\SourceCodePro.ttf")) {
	            throw new std::exception("Couldn't load font");
	        }

	        text.setFont(font);
	        text.setString("Oh hai, press Y to pray game.\nPress N to kill game.");
	        text.setCharacterSize(24);
	        text.setColor(sf::Color::Cyan);
	        text.setPosition({ 40.0f, 40.0f });
        }

        void update(BaseState &circleStateBase, GameState &gameState)
        {
            gameState.renderer->draw(text);
        }
};
