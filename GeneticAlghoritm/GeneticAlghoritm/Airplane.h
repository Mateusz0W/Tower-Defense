#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Airplane {
public:
	Airplane() = default;
	Airplane(const std::string& AirplaneLocalization, sf::Vector2f startingPosition,float speed):_speed(speed) {
		AirplaneTexture.loadFromFile(AirplaneLocalization);
		AirplaneSprite.setTexture(AirplaneTexture);
		AirplaneSprite.setPosition(startingPosition);
		AirplaneSprite.setScale(0.4, 0.4);
		AirplaneSprite.setOrigin(AirplaneSprite.getLocalBounds().width / 2, AirplaneSprite.getLocalBounds().height / 2);// set the reference point to the center of the object
	}
	sf::Texture AirplaneTexture;
	sf::Sprite AirplaneSprite;
	void fly(float deltaTime);
	float ReturnSpeed();
	void Restart();
private:
	float _speed;

};