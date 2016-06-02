#include "Logic.h"

#include "Interpolation.h"
#include "State.h"
#include "Input.h"

// === Logic

    Logic::Logic(sf::Time logicTickDelta) :
        logicTickDelta(logicTickDelta), stateStack()
    {
        // auto selfPointer = nullptr; // std::make_shared<Logic>(this);
        auto eventQueue = std::make_shared<EventQueue>(*this);
        this->eventQueue = eventQueue;
        // this->currentState = std::make_shared<CircleState>(logicTickDelta, eventQueue);
        stateStack.push(std::make_shared<MenuScreenState>(logicTickDelta, eventQueue));
        // stateStack.push(std::make_shared<PlayingGameState>(logicTickDelta, eventQueue));
    }

    bool Logic::update(sf::Time timeElapsed, std::function<Input()> getInput)
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

    void Logic::render(sf::RenderWindow &renderer)
    {
        renderer.clear();

        float interpolationFactor = lag / logicTickDelta;

        getCurrentState()->render(renderer, interpolationFactor);

        renderer.display();
    }