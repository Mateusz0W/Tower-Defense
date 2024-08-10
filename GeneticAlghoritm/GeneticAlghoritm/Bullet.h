#pragma once

#include"Tower.h"
#include "Airplane.h"

class Bullet:public Tower {
public:
	Bullet() = default;
	Bullet(float speed):_x(730.f),_y(890.f),_speed(speed) {
		bullet.setRadius(10);
		bullet.setFillColor(sf::Color::Red);
		
	}
	sf::Vector2f setBulletStartingPosition(Tower &tower,double angle);
	sf::CircleShape bullet;
	void fly(float deltaTime,double angle);
	bool ReachAirplaneHeight(Airplane &airplane);
	float DistanceToAirplane(Airplane& airplane);
	float ReturnSpeed();
private:
	double _x;
	double _y;
	float _speed;
	
};