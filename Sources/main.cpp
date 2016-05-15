#include "main.h"
#include "Logic.h"

#include <thread>

sf::CircleShape circle(50.0f);
sf::Vector2f currentCirclePosition { 20.0f, 20.0f },
             nextCirclePosition = currentCirclePosition,
	         circleDirection = { 0.0f, 0.0f };
float circleSpeed = 400.0f; // pix / sec
sf::Time logicTickDelta = sf::milliseconds(10);

Input input;

void updateInput(sf::Window &window) {
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Up:    circleDirection.y = -1.0f; break;
			case sf::Keyboard::Down:  circleDirection.y = 1.0f; break;
			case sf::Keyboard::Left:  circleDirection.x = -1.0f; break;
			case sf::Keyboard::Right: circleDirection.x = 1.0f; break;
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			case sf::Keyboard::Up:    circleDirection.y = 0.0f; break;
			case sf::Keyboard::Down:  circleDirection.y = 0.0f; break;
			case sf::Keyboard::Left:  circleDirection.x = 0.0f; break;
			case sf::Keyboard::Right: circleDirection.x = 0.0f; break;
			}
		}
	}
}

void logicTick() {
	auto deltaPosition = circleDirection * circleSpeed * logicTickDelta.asSeconds();
	
	currentCirclePosition = nextCirclePosition;
	nextCirclePosition = currentCirclePosition + circleDirection * circleSpeed * logicTickDelta.asSeconds();
}

template<typename T>
T interpolate(T from, T to, float factor) {
	return from * factor + to * (1 - factor);
}

/*template<>
sf::Vector2f interpolate(sf::Vector2f from, sf::Vector2f to, float factor) {
	return from * factor + to * (1 - factor);
}*/

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Hello SFML");
	
	sf::Clock clock;
	sf::Time lag;

	circle.setPosition(currentCirclePosition);
	circle.setFillColor(sf::Color::Magenta);

	while (window.isOpen()) {
		sf::Time delta = clock.restart();
		lag += delta;

		// std::cout << "New lag: " << lag.asMilliseconds() << "ms (added " << delta.asMilliseconds() << "ms)" << std::endl;

		while (lag > logicTickDelta) {
			updateInput(window);
			logicTick();

			lag -= logicTickDelta;
			// std::cout << "Frame! Lag left: " << lag.asMilliseconds() << "ms" << std::endl;
		}

		window.clear();

		float interpolationFactor = lag / logicTickDelta;

		circle.setPosition(interpolate(currentCirclePosition, nextCirclePosition, interpolationFactor));

		window.draw(circle);

		window.display();
	}

	char c;
	std::cin >> c;
}