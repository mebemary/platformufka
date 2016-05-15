#pragma once
#include <SFML/System.hpp>
#include "Interpolation.h"

struct Input
{
	bool jump, crouch, left, right, yes, no;
};

class Logic
{
		sf::Time lag,
			     logicTickDelta;
		sf::Vector2f currentCirclePosition{ 20.0f, 20.0f },
					 nextCirclePosition = currentCirclePosition;
		float circleSpeed = 400.0f; // pix / sec

		void logicTick(Input input) {
			sf::Vector2f circleDirection {input.left * -1.0f + input.right * 1.0f,
										  input.jump * -1.0f + input.crouch * 1.0f};
			auto deltaPosition = circleDirection * circleSpeed * logicTickDelta.asSeconds();

			currentCirclePosition = nextCirclePosition;
			nextCirclePosition = currentCirclePosition + circleDirection * circleSpeed * logicTickDelta.asSeconds();
		}

	public:
		Logic(sf::Time logicTickDelta) :
			logicTickDelta(logicTickDelta)
		{
	
		}

		void update(sf::Time timeElapsed, Input input)
		{
			lag += timeElapsed;

			while (lag > logicTickDelta) {			
				logicTick(input);

				lag -= logicTickDelta;
			}

		}

		sf::Vector2f getCirclePosition()
		{
			float interpolationFactor = lag / logicTickDelta;

			return interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor);
		}
};

