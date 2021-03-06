#include "State.h"

#include "EventQueue.h"
#include "Input.h"
#include "GameObject.h"

#include "GameObjects/Circle.h"
#include "GameObjects/Floor.h"
#include "GameObjects/Menu.h"
#include "GameObjects/Enemy.h"

// =====================

    State &State::addGameObject(std::shared_ptr<BaseGameObject> gameObject, std::string tag)
    {
        gameObjects.push_back(std::make_tuple(tag, gameObject));
        return *this;
    }

    State::State(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
        tickDelta(tickDelta)
    {
        this->eventQueue = eventQueue;
	}

    std::vector<std::shared_ptr<BaseGameObject>> State::getGameObjectsByTag(std::string tag)
    {
        std::vector<std::shared_ptr<BaseGameObject>> result;

        for (auto &pair : gameObjects) {
            auto pairTag = std::get<0>(pair);
            if (pairTag == tag) {
                auto ptr = std::get<1>(pair);
                result.push_back(ptr);
            }
        }

        return result;
    }

    void State::update(Input input)
    {
        this->input = input;

        GameState gameState;
        gameState.input = this->input;
        gameState.tickDelta = this->tickDelta;
        gameState.currentState = this;
        
        for (auto &gameObjectPair : gameObjects)
        {
            std::get<1>(gameObjectPair)->update(gameState);
        }
    }

    void State::render(sf::RenderWindow &renderer, float interpolationFactor)
    {
        GameState gameState;
        gameState.renderer = &renderer;

        for (auto &gameObjectPair : gameObjects)
        {
            std::get<1>(gameObjectPair)->render(gameState);
        }
    }

// ======================

    PlayingGameState::PlayingGameState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
        State(tickDelta, eventQueue)
    {
        auto pinkCircle = std::make_shared<GameObject<CircleState>>();
        auto floor = std::make_shared<GameObject<FloorState>>();
        auto enemy = std::make_shared<GameObject<EnemyState>>();


        (*pinkCircle).addComponent("inputComponent", std::make_unique<CircleInputComponent>())
                     .addComponent("physicsComponent", std::make_unique<CirclePhysicsComponent>())
                     .addComponent("graphicsComponent", std::make_unique<CircleGraphicsComponent>());

        floor->addComponent("graphicsComponent", std::make_unique<FloorGraphicsComponent>());

        (*enemy).addComponent("physicsComponent", std::make_unique<EnemyPhysicsComponent>())
                .addComponent("graphicsComponent", std::make_unique<EnemyGraphicsComponent>());

        addGameObject(floor, "floor");
        addGameObject(pinkCircle, "player");
        addGameObject(enemy, "enemy");
    }

// ====================

    MenuScreenState::MenuScreenState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
        State(tickDelta, eventQueue)
    {
        auto menu = std::make_shared<GameObject<MenuState>>();

        (*menu).addComponent("inputComponent", std::make_unique<MenuInputComponent>())
               .addComponent("graphicsComponent", std::make_unique<MenuGraphicsComponent>());

        addGameObject(menu);
    }

// ====================

    // UrDedScreenState::UrDedScreenState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
    //     State(tickDelta, eventQueue)
    // {
    //     auto urDedScreen = std::make_shared<GameObject<UrDedState>>();

    //     (*urDedScreen).addComponent("inputComponent", std::make_unique<UrDedInputComponent>())
    //                   .addComponent("graphicsComponent", std::make_unique<UrDedGraphicsComponent>());

    //     addGameObject(urDedScreen);
    // }