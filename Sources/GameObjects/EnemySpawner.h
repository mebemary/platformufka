#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"
#include "Enemy.h"
#include <random>

struct EnemySpawnerState : public BaseState
{
	sf::Time elapsed;
    sf::Time lastSpawn;
};

class EnemySpawnerComponent : public Component<EnemySpawnerState>
{
        sf::Time nextInterval;;
        sf::Vector2f spawnPosition = { 900.0f, 400.0f };
        std::random_device rd;
        std::mt19937 randomGenerator;
        std::uniform_real_distribution<> randomDistribution;

        std::shared_ptr<GameObject<EnemyState>> newEnemy()
        {
            auto enemy = std::make_shared<GameObject<EnemyState>>();

            (*enemy).addComponent("physicsComponent", std::make_unique<EnemyPhysicsComponent>())
                    .addComponent("graphicsComponent", std::make_unique<EnemyGraphicsComponent>());

            return enemy;
        }

    public:
        EnemySpawnerComponent() :
            rd(), randomGenerator(rd), randomDistribution(2, 4)
        {
            nextInterval = sf::seconds(randomDistribution(randomGenerator));
        }

	    void update(BaseState &spawnerStateBase, GameState &gameState)
        {
            EnemySpawnerState &spawnerState = reinterpret_cast<EnemySpawnerState &>(spawnerStateBase);

            spawnerState.elapsed += gameState.getTickDelta();

            if (spawnerState.elapsed.asMilliseconds() >= spawnerState.lastSpawn.asMilliseconds() + nextInterval.asMilliseconds()) {
                spawnerState.lastSpawn = spawnerState.elapsed;
                nextInterval = sf::seconds(randomDistribution(randomGenerator));

                gameState.addGameObject(newEnemy(), "enemy");
            }
        }
};