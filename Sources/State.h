#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <exception>
#include <memory>
#include <cmath>

#include "EventQueue.h"
#include "Input.h"
#include "GameObject.h"

#include "GameObjects/Circle.h"
#include "GameObjects/Floor.h"

class State {
	protected:
		sf::Time tickDelta;
		std::shared_ptr<EventQueue> eventQueue;

	public:
        State(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
            tickDelta(tickDelta)
        {
            this->eventQueue = eventQueue;
		}

		virtual void update(Input input) = 0;
		virtual void render(sf::RenderWindow &renderer, float interpolationFactor) = 0;
};


class PlayingGameState : public State {
        std::shared_ptr<GameObject<CircleState>> pinkCircle;
        std::shared_ptr<GameObject<FloorState>> floor;
        Input input;

	public:
        PlayingGameState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
            State(tickDelta, eventQueue)
		{
            pinkCircle = std::make_shared<GameObject<CircleState>>();
            floor = std::make_shared<GameObject<FloorState>>();


            (*pinkCircle).addComponent("inputComponent", std::make_unique<CircleInputComponent>())
                         .addComponent("physicsComponent", std::make_unique<CirclePhysicsComponent>())
                         .addComponent("graphicsComponent", std::make_unique<CircleGraphicsComponent>());

            floor->addComponent("graphicsComponent", std::make_unique<FloorGraphicsComponent>());
		}

		void update(Input input)
		{
            this->input = input;
            if (input.no == KeyState::RELEASED) {
                eventQueue->popState();
                return;
            }

            GameState gameState;
            gameState.input = this->input;
            gameState.tickDelta = this->tickDelta;
            gameState.floor = floor;

            pinkCircle->update(gameState);
		}

		virtual void render(sf::RenderWindow &renderer, float interpolationFactor)
		{
            GameState gameState;
            gameState.renderer = &renderer;

            pinkCircle->render(gameState);
            floor->render(gameState);
		}
};

class MenuState : public State {
		sf::Font font;
		sf::Text text;

	public:
        MenuState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
            State(tickDelta, eventQueue)
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

		void update(Input input)
		{
            if (input.yes == KeyState::RELEASED) {
                eventQueue->pushState(std::make_shared<PlayingGameState>(tickDelta, eventQueue));
            }
            else if (input.no == KeyState::RELEASED) {
                eventQueue->exitGame();
            }
		}

		virtual void render(sf::RenderWindow &renderer, float interpolationFactor)
		{
			renderer.draw(text);
		}
};