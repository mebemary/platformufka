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

        std::vector<std::shared_ptr<BaseGameObject>> GameState::getGameObjectsByTag(std::string tag)
        {   
            return currentState->getGameObjectsByTag(tag);
        }
