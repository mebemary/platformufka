#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <map>
#include <vector>
#include <algorithm>

#include "Input.h"

class State;

struct BaseState
{

};

class GameState;

class BaseComponent
{
    public:
        virtual void update(BaseState &, GameState &) = 0;
};

class BaseGameObject
{
    public:
        virtual BaseState &getState() = 0;
        virtual std::map<std::string, std::unique_ptr<BaseComponent>> &getComponents() = 0;

        void update(GameState &gameState);
        void render(GameState &gameState);
};

typedef std::tuple<std::string, std::shared_ptr<BaseGameObject>> GameObjectListPair;

class GameState
{
        std::vector<GameObjectListPair> gameObjects;

    public:
        Input input;
        sf::Time tickDelta;
        sf::RenderWindow *renderer;
        State *currentState;

        std::vector<std::shared_ptr<BaseGameObject>> getGameObjectsByTag(std::string tag);
};

template <typename StateT>
class Component : public BaseComponent
{
    //  public:
    //    virtual void update(StateT &, GameState &) = 0;
};

template <typename StateT>
class GameObject : public BaseGameObject
{
        StateT state; 
        std::map<std::string, std::unique_ptr<BaseComponent>> components;
            
    public:
        StateT &getState() { return state; }
        std::map<std::string, std::unique_ptr<BaseComponent>> &getComponents() { return components; }

        GameObject &addComponent(std::string name, std::unique_ptr<Component<StateT>> component)
        {
            components[name] = std::move(component);
            return *this;
        }
};