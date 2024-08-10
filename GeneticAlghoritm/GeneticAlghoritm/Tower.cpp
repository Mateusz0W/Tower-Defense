#include "Tower.h"

std::pair <double, float> Tower::ReturnAngleAndTimeToShoot() {
	return std::make_pair(_angle, _time_to_shoot);
}
void Tower::Set(double angle, float time) {
	_angle = angle;
	_time_to_shoot = time;
	_shoot = false;
}
void Tower::Fire() {
	_shoot = true;
}

