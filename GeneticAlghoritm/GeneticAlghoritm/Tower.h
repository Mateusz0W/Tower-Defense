
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include<iostream>

class Tower {
public:
	Tower() = default;
	Tower(const std::string& BaseLocalization, const std::string& GunLocalization, sf::Vector2f BaseStartingPosition, sf::Vector2f GunStartingPosition) {
		BaseTexture.loadFromFile(BaseLocalization);
		GunTexture.loadFromFile(GunLocalization);
		BaseSprite.setTexture(BaseTexture);
		GunSprite.setTexture(GunTexture);
		BaseSprite.setPosition(BaseStartingPosition);
		GunSprite.setPosition(GunStartingPosition);
		BaseSprite.setScale(0.4, 0.4);
		GunSprite.setScale(0.4, 0.4);
		GunSprite.setOrigin(GunSprite.getLocalBounds().width / 2, GunSprite.getLocalBounds().height / 2);// set the reference point to the center of the object
		_shoot = false;
	}
	sf::Texture BaseTexture;
	sf::Sprite BaseSprite;
	sf::Texture GunTexture;
	sf::Sprite GunSprite;
	std::pair <double, float> ReturnAngleAndTimeToShoot();
	void Set(double angle,float time);
	void Fire();
protected:
	double _angle;
	float _time_to_shoot;
	bool _shoot;


};