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
            getComponents().at("graphicsComponent")->update(getState(), gameState);
        }

// === GameState

        /*GameState(State *curentState) :
            currentState(currentState)
        {

        }*/

        /*GameState(State *curentState, sf::RenderWindow &renderer) :
            renderer(renderer), GameState(currentState)
        {

        }*/

        std::vector<std::shared_ptr<BaseGameObject>> GameState::getGameObjectsByTag(std::string tag)
        {   
            return currentState->getGameObjectsByTag(tag);
        }

        GameState &GameState::addGameObject(std::shared_ptr<BaseGameObject> gameObject, std::string tag)
        {
            currentState->addGameObject(gameObject, tag);
            return *this;
        }

        /*GameState &GameState::removeGameObject(std::shared_ptr<BaseGameObject> gameObject)
        {
            currrentState->removeGameObject(gameObject);
            return *this;
        }*/

        template<typename StateT>
        GameState &GameState::pushState()
        {
            auto state = std::make_shared<PlayingGameState>(tickDelta, eventQueue);
            currentState->eventQueue->statePush(state);
            return *this;
        }

        GameState &GameState::popState()
        {
            currentState->eventQueue->statePop();
            return *this;
        }

        GameState &GameState::exitGame()
        {
            currentState->eventQueue->statePop();
            return *this;
        }

        GameState &GameState::render(sf::Drawable &drawable)
        {
            currentState->renderer->draw(drawable);
            return *this;
        }