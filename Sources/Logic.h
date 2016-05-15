#pragma once

#include <SFML/System.hpp>
#include "Interpolation.h"
#include "State.h"
#include "Input.h"

class Logic
{
		sf::Time lag,
			     logicTickDelta;
		std::shared_ptr<State> currentState;

	public:
		Logic(sf::Time logicTickDelta) :
			logicTickDelta(logicTickDelta)
		{
			currentState = std::make_shared<CircleState>(logicTickDelta);
		}

		void update(sf::Time timeElapsed, Input input)
		{
			lag += timeElapsed;

			while (lag > logicTickDelta) {			
				currentState->update(input);

				lag -= logicTickDelta;
			}
		}

		void render(sf::RenderWindow &renderer)
		{
			renderer.clear();

			float interpolationFactor = lag / logicTickDelta;

			currentState->render(renderer, interpolationFactor);

			renderer.display();
		}
};

