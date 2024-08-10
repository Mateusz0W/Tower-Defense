#pragma once

#include<vector>
#include "Bullet.h"
#include<algorithm>
#include<type_traits>

#define POPULATION_SIZE 200
#define MUTATION_RATE 0.7
#define M_PI 3.14159265358979323846

namespace GEN {

	const float Target_distance = 0.1f; // distance between bullet and airplane we want to achieve
	const float Target_time = 2; //time below which we want to fire bullet

	struct Individual {
		double angle;
		float time_to_shoot;
		float fit;// distance between airplane and bullet. if fit is lower it is better
		float probability;
		Individual() = default;
		Individual(double angle, float time_to_shoot) :angle(angle), time_to_shoot(time_to_shoot) {}
	};

	template<typename T=int>
	T RandomValues(T MIN, T MAX) {
		/*
		Statment: MIN + static_cast<T>(rand()) / static_cast<T>(RAND_MAX) * (MAX - MIN); 
		dosen't work well for integer variable because static_cast rounds number down so this statment
		always returns 0. That's why we need statment: MIN + rand() % (MAX - MIN + 1);
		*/
		if constexpr (std::is_integral<T>::value)// this statment checks if T is integer variable
			return MIN + rand() % (MAX - MIN + 1);
		else
			return MIN + static_cast<T>(rand()) / static_cast<T>(RAND_MAX) * (MAX - MIN);
	}
	std::vector<Individual> GeneratePopulation() {
		std::vector<Individual> population;

		for (int i = 0; i < POPULATION_SIZE; i++)
			population.push_back(Individual(RandomValues<double>(0.0, 360.0), RandomValues<float>(0.f, 10.f)));//(0.0,360) -> from zero degrees to 360 .(0.f, 10.f) -> from 0 seconds to 10

		return population;
	}
	float fitness(Bullet& bullet, Tower& tower, Airplane &airplane,Individual individual) {
		const float x0 = bullet.setBulletStartingPosition(tower,individual.angle).x;
		const float y0 = bullet.setBulletStartingPosition(tower,individual.angle).y;
		const float y1 = airplane.AirplaneSprite.getPosition().y;
		const float x1= airplane.AirplaneSprite.getPosition().x;

		const double angle = individual.angle;
		const float time_to_shoot = individual.time_to_shoot;
		const float poor_fit = pow(10.f, 20);

		const float v_bullet = bullet.ReturnSpeed();
		const float v_airplane = airplane.ReturnSpeed();

		float sin_angle = sin(angle * M_PI / 180.0);
		float bullet_fly_time;

		if (angle > 180) // in this case sin(angle) is always negative so movement is in down direction
			return poor_fit;

		if (sin_angle != 0)
			bullet_fly_time = (y0 - y1) / (v_bullet * sin_angle);
		else
			return poor_fit;

		if (bullet_fly_time < 0)
			return poor_fit;

		float x_bullet_end = x0 + v_bullet * cos(angle * M_PI / 180.0) * bullet_fly_time*-1;
		float  x_airplane_end = x1 + v_airplane * (bullet_fly_time + time_to_shoot);

		return abs(x_airplane_end - x_bullet_end);
	}
	Individual Selection(std::vector<Individual>& population) {

		std::vector<float> cumulative_probability;
		float cumulative_sum = 0;

		for (auto& individual : population) {
			cumulative_sum += individual.probability;
			cumulative_probability.push_back(cumulative_sum);
		}

		float random = RandomValues<float>(0, 1);

		for (int i = 0; i < cumulative_probability.size(); i++) {
			if (cumulative_probability[i] > random)
				return population[i];
		}
		return population.back();

	}
	std::pair<Individual, Individual> crossover(Individual parent1, Individual parent2) {
		double angle1 = parent1.angle;
		float  time1 = parent1.time_to_shoot;

		double angle2 = parent2.angle;
		float time2 = parent2.time_to_shoot;

		int random = RandomValues(0, 1);

		Individual child1;
		Individual child2;

		if (random == 0) {
			child1 = Individual(angle1, time2);
			child2 = Individual(angle2, time1);
		}
		else {
			child1 = Individual(angle2, time1);
			child2 = Individual(angle1, time2);
		}
		return std::make_pair(child1, child2);
	}
	void mutation(Individual& individual) {
		if (RandomValues<double>(0.0, 1.0) <= MUTATION_RATE) {
			individual.angle += RandomValues<double>(-10.0, 10.0);
			if (individual.angle > 360.0)
				individual.angle -= 360; 
			else if (individual.angle < 0.0)
				individual.angle = 360 + individual.angle;
		}
		if (RandomValues<double>(0.0, 1.0) <= MUTATION_RATE) {
			individual.time_to_shoot += RandomValues<float>(-10.0f, 10.0f);
			if (individual.time_to_shoot < 0)
				individual.time_to_shoot = 0;
		}
	}
	bool found(Individual& individual) {
		if (individual.fit < Target_distance && individual.time_to_shoot < Target_time)
			return true;
		else
			return false;
	}
}

