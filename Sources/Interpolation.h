#pragma once

template<typename T>
T interpolate(T from, T to, float factor) {
	return from * factor + to * (1 - factor);
}

/*template<>
sf::Vector2f interpolate(sf::Vector2f from, sf::Vector2f to, float factor) {
return from * factor + to * (1 - factor);
}*/
