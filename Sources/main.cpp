#include "main.h"
#include "Logic.h"

#include <thread>

sf::CircleShape circle(50.0f);

const sf::Time LOGIC_TICK_DELTA = sf::milliseconds(10);

Input input;

void updateInput(sf::Window &window) {
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Up:    input.jump=true; break;
			case sf::Keyboard::Down:  input.crouch=true; break;
			case sf::Keyboard::Left:  input.left=true; break;
			case sf::Keyboard::Right: input.right=true; break;
			case sf::Keyboard::Y:	  input.yes = true; break;
			case sf::Keyboard::N:     input.no = true; break;
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			case sf::Keyboard::Up:    input.jump = false; break;
			case sf::Keyboard::Down:  input.crouch = false; break;
			case sf::Keyboard::Left:  input.left = false; break;
			case sf::Keyboard::Right: input.right = false; break;
			case sf::Keyboard::Y:	  input.yes = false; break;
			case sf::Keyboard::N:     input.no = false; break;
			}
		}
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Hello SFML");
	
	sf::Clock clock;
	Logic logic(LOGIC_TICK_DELTA);

	circle.setFillColor(sf::Color::Magenta);
	circle.setPosition({ 20.0f, 20.0f });

	while (window.isOpen()) {
		sf::Time delta = clock.restart();
		
		updateInput(window);
		
		logic.update(delta, input);

		window.clear();

		circle.setPosition(logic.getCirclePosition());

		window.draw(circle);

		window.display();
	}

	char c;
	std::cin >> c;
}