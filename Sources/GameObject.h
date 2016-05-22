#pragma once

#include <memory>
#include <map>

#include "Input.h"

template <typename StateT>
class GameObject;

class FloorState;

class GameState
{
    public:
        Input input;
        sf::Time tickDelta;
        sf::RenderWindow *renderer;
        std::shared_ptr<GameObject<FloorState>> floor;

        std::shared_ptr<GameObject<FloorState>> getFloor()
        {
            return floor;
        }
};

template <typename StateT>
class Component
{
    public:
        virtual void update(StateT &, GameState &) = 0;
};

template <typename StateT>
class GameObject
{
        std::map<std::string, std::unique_ptr<Component<StateT>>> components;
            
    public:
        StateT state;

        virtual void update(GameState &gameState)
        {
            for (auto &pair : components)
            {
                if (pair.first == "graphicsComponent") continue;
                pair.second->update(state, gameState);
            }
        }

        virtual void render(GameState &gameState)
        {
            components.at("graphicsComponent")->update(state, gameState);
        }

        GameObject &addComponent(std::string name, std::unique_ptr<Component<StateT>> component)
        {
            components[name] = std::move(component);
            return *this;
        }
};
