#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <exception>
#include <memory>

#include "EventQueue.h"
#include "Input.h"

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


class CircleState : public State {
		sf::CircleShape circle;
		sf::Vector2f currentCirclePosition{ 20.0f, 20.0f },
			nextCirclePosition = currentCirclePosition;
		float circleSpeed = 400.0f; // pix / sec

	public:
        CircleState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
            State(tickDelta, eventQueue), circle(50.0f)
		{
			circle.setFillColor(sf::Color::Magenta);

		}

		void update(Input input)
		{
            if (input.no) {
                // TODO: with input being reused across a few logic ticks this may pop a couple of states; fix it
                eventQueue->popState();
                return;
            }

			sf::Vector2f circleDirection{ input.left * -1.0f + input.right * 1.0f,
				input.jump * -1.0f + input.crouch * 1.0f };
			
			auto deltaPosition = circleDirection * circleSpeed * tickDelta.asSeconds();

			currentCirclePosition = nextCirclePosition;
			nextCirclePosition = currentCirclePosition + circleDirection * circleSpeed * tickDelta.asSeconds();
		}

		virtual void render(sf::RenderWindow &renderer, float interpolationFactor)
		{
			circle.setPosition(interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor));
			renderer.draw(circle);
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
            if (input.yes) {
                eventQueue->pushState(std::make_shared<CircleState>(tickDelta, eventQueue));
            }
            else if (input.no) {
                eventQueue->exitGame();
            }
		}

		virtual void render(sf::RenderWindow &renderer, float interpolationFactor)
		{
			renderer.draw(text);
		}
};