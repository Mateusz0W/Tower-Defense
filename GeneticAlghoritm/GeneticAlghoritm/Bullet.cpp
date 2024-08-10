#include "Bullet.h"
#include <cmath>

#define M_PI           3.14159265358979323846

void Bullet::fly(float deltaTime,double angle) {
	std::cout<<Tower::_angle<<std::endl;
	if (Tower::_shoot) {
		sf::Vector2f position = bullet.getPosition();
		position.x += _speed * cos(angle * M_PI / 180.0) * deltaTime *-1;
		position.y += _speed * sin(angle * M_PI / 180.0) * deltaTime *-1 ;//Tower::_angle
		bullet.setPosition(position);
	}
}
sf::Vector2f Bullet::setBulletStartingPosition(Tower &tower,double angle) {
	double x0 = tower.GunSprite.getPosition().x;
	double y0 = tower.GunSprite.getPosition().y;

	double r = sqrt(pow(_x - x0, 2) + pow(_y - y0, 2));
	
	double alpha = atan2(_y - y0, _x - x0);
	double phi = alpha + (angle * M_PI/ 180.0);
	
	double x_result = x0 + r * cos(phi);
	double y_result = y0 + r * sin(phi);
	
	return sf::Vector2f(x_result, y_result);
}
bool Bullet::ReachAirplaneHeight(Airplane& airplane) {
	float elipson = 5.0f;
	if (abs(bullet.getPosition().y - airplane.AirplaneSprite.getPosition().y) < elipson)
		return true;
	else
		return false;
}
float Bullet::DistanceToAirplane(Airplane& airplane) {
		return abs(bullet.getPosition().x - airplane.AirplaneSprite.getPosition().x);
}
float Bullet::ReturnSpeed() {
	return _speed;
}
