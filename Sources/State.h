#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <exception>
#include <memory>
#include <cmath>
#include <tuple>
#include <list>

#include "Input.h"

class EventQueue;
class BaseGameObject;

typedef std::tuple<std::string, std::shared_ptr<BaseGameObject>> GameObjectListPair;

class State {
    protected:
        sf::Time tickDelta;
        std::list<GameObjectListPair> gameObjects;
        Input input;

    public:
        // TODO: procxy methods here
        std::shared_ptr<EventQueue> eventQueue;

        State(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue);

        std::vector<std::shared_ptr<BaseGameObject>> getGameObjectsByTag(std::string tag);

        void update(Input input);

        void render(sf::RenderWindow &renderer, float interpolationFactor);

        State &addGameObject(std::shared_ptr<BaseGameObject> gameObject, std::string tag = "");
        State &removeGameObject(std::shared_ptr<BaseGameObject> gameObject);
};


class PlayingGameState : public State {
    public:
        PlayingGameState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue);
};

class MenuScreenState : public State {
    public:
        MenuScreenState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue);
};

// class UrDedScreenState : public State {
//     public:
//         UrDedScreenState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue);
// };