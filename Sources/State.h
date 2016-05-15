#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Logic.h"
#include "Input.h"

class State {
	protected:
		sf::Time tickDelta;

	public:
		State(sf::Time tickDelta) :
			tickDelta(tickDelta) {

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
		CircleState(sf::Time tickDelta) :
			State(tickDelta), circle(50.0f)
		{
			circle.setFillColor(sf::Color::Magenta);

		}

		void update(Input input)
		{
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
	public:
		MenuState(sf::Time tickDelta) :
			State(tickDelta), circle(50.0f)
		{
			circle.setFillColor(sf::Color::Magenta);

		}

		void update(Input input)
		{
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