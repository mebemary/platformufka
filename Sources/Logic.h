#pragma once
#include <SFML/System.hpp>

struct Input
{
	bool jump, crouch, left, right, yes, no;
};

class Logic
{
		sf::Time lag, logicTickDelta;

		void logicTick()
		{

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
				// logicTick();

				lag -= logicTickDelta;
			}

		}
};

