#pragma once

#include <SFML/System.hpp>
#include <memory>
#include <stack>
#include <functional>

#include "Interpolation.h"
#include "State.h"
#include "Input.h"

class Logic : public IStateStackable //, public IGameExitable
{
        sf::Time lag,
                 logicTickDelta;
        std::shared_ptr<EventQueue> eventQueue;
        std::stack<std::shared_ptr<State>> stateStack;
        bool gameRunning = true;

        inline std::shared_ptr<State> getCurrentState() {
            return stateStack.top();
        }

    public:
        Logic(sf::Time logicTickDelta) :
            logicTickDelta(logicTickDelta), stateStack()
        {
            // auto selfPointer = nullptr; // std::make_shared<Logic>(this);
            auto eventQueue = std::make_shared<EventQueue>(*this);
            this->eventQueue = eventQueue;
            // this->currentState = std::make_shared<CircleState>(logicTickDelta, eventQueue);
            stateStack.push(std::make_shared<MenuState>(logicTickDelta, eventQueue));
        }

        bool update(sf::Time timeElapsed, std::function<Input()> getInput)
        {
            lag += timeElapsed;

            while (lag > logicTickDelta) {
                auto input = getInput();
                getCurrentState()->update(input);

                if (!gameRunning) { return false; }

                lag -= logicTickDelta;
            }

            return true;
        }

        void render(sf::RenderWindow &renderer)
        {
            renderer.clear();

            float interpolationFactor = lag / logicTickDelta;

            getCurrentState()->render(renderer, interpolationFactor);

            renderer.display();
        }

        inline void pushState(std::shared_ptr<State> newState)
        {
            stateStack.push(newState);
        }

        inline void popState()
        {
            stateStack.pop();
        }

        inline void exitGame()
        {
            gameRunning = false;
        }
};

