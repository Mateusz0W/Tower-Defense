#include "Airplane.h"

void Airplane::fly(float deltaTime) {

	sf::Vector2f position = AirplaneSprite.getPosition();
	position.x += _speed * deltaTime;// s=v*t
	AirplaneSprite.setPosition(position);
}
float Airplane::ReturnSpeed() {
	return _speed;
}
void Airplane::Restart() {
	this->AirplaneSprite.setPosition(-250, 100);
}