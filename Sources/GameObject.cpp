#pragma once

#include <memory>
#include <map>
#include <vector>
#include <algorithm>

#include "Input.h"
#include "GameObject.h"
#include "State.h"

// ==== BaseGameObject

        void BaseGameObject::update(GameState &gameState)
        {
            for (auto &pair : getComponents())
            {
                if (pair.first == "graphicsComponent") continue;
                pair.second->update(getState(), gameState);
            }
        }

        void BaseGameObject::render(GameState &gameState)
        {
            auto graphicsComponent = getComponents().find("graphicsComponent");
                
            if (graphicsComponent != getComponents().end()) {
                (*graphicsComponent).second->update(getState(), gameState);
            }
        }

// === GameState

        GameState::GameState(State *currentState) :
            currentState(currentState), renderer(nullptr)
        {

        }

        GameState::GameState(State *currentState, sf::RenderWindow *renderer) :
            currentState(currentState), renderer(renderer)
        {

        }

        Input GameState::getInput()
        {
            return currentState->getInput();
        }

        sf::Time GameState::getTickDelta()
        {
            return currentState->getTickDelta();
        }

        std::vector<std::shared_ptr<BaseGameObject>> GameState::getGameObjectsByTag(std::string tag)
        {   
            return currentState->getGameObjectsByTag(tag);
        }

        GameState &GameState::addGameObject(std::shared_ptr<BaseGameObject> gameObject, std::string tag)
        {
            currentState->addGameObject(gameObject, tag);
            return *this;
        }

        GameState &GameState::removeGameObject(std::shared_ptr<BaseGameObject> gameObject)
        {
            currentState->removeGameObject(gameObject);
            return *this;
        }

        GameState &GameState::popState()
        {
            currentState->eventQueue->popState();
            return *this;
        }

        GameState &GameState::exitGame()
        {
            currentState->eventQueue->exitGame();
            return *this;
        }

        GameState &GameState::render(sf::Drawable &drawable)
        {
            renderer->draw(drawable);
            return *this;
        }