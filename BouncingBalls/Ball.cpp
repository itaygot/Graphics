#include "Ball.h"
#include "glm/geometric.hpp"	// normalize(); length(); dot();
#include <stdio.h>

Ball::Ball(float x, float y, float radius) : _pos(x, y), _radius(radius), 
	_bounciness(1.f), _static(false) {}

Ball::Ball(float x, float y) : _pos(x, y), _bounciness(1.f), 
	_static(false) {}

void Ball::ballCollision(Ball& other)
{
	glm::vec2 axis;
	float dist, hit_axis_speed;

	if ((dist = touchingBall(other)) > 0.f) {	// Collision
		axis = (other._pos - _pos) / dist;
		hit_axis_speed = glm::dot(_velo, axis) - glm::dot(other._velo, axis);

		if (hit_axis_speed > 0) {	// going at eachother
			if (!_static && !other._static) {		// static-static collision
				_velo -= axis * hit_axis_speed * _bounciness;
				other._velo += axis * hit_axis_speed * other._bounciness;
			}
			else if (!_static == other._static) {	// static-non static collision
				if(!_static)
					_velo -= 2.f * axis * hit_axis_speed * _bounciness;
				else
					other._velo += 2.f * axis * hit_axis_speed * other._bounciness;
			}
		}

	}

	
}

float Ball::touchingBall(const Ball & other) {
	float dist;

	// first, filter far away balls
	if ((_pos.x - _radius <= other._pos.x + other._radius) &&
		(_pos.x + _radius >= other._pos.x - other._radius) &&
		(_pos.y - _radius <= other._pos.y + other._radius) &&
		(_pos.y + _radius >= other._pos.y - other._radius))
	{
		dist = glm::length(other._pos - _pos);
		if (dist <= _radius + other._radius)		// balls touching
			return dist;
	}

	return -1.f;

}
