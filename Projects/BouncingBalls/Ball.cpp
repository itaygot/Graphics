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
	float dist, hitAxisSpeed;
	//using glm::dot;


	// Check if touching and collision axis
	if ((dist = touchingBallDistance(other)) <= 0.f)		// balls not touching
		return;

	// Hit axis
	axis = (other._pos - _pos) / dist;
	hitAxisSpeed = dot(_velo, axis) - dot(other._velo, axis);

	// Check that going at eachother along the axis
	if (hitAxisSpeed <= 0)
		return;

	// Static vs. Non-Static collision
	if (!_static && !other._static) {
		_velo -= axis * hitAxisSpeed * _bounciness;
		other._velo += axis * hitAxisSpeed * other._bounciness;
	}

	else if (!_static == other._static) {
		if (!_static)
			_velo -= axis * 2.f * hitAxisSpeed * _bounciness;
		else
			other._velo += axis * 2.f * hitAxisSpeed * other._bounciness;
	}
	
}

bool Ball::containsPoint(float x, float y) {
	
	using glm::vec2;
	using glm::dot;

	// Filter far points
	if (x + _radius < _pos.x ||
		x - _radius > _pos.x ||
		y + _radius < _pos.y ||
		y - _radius > _pos.y)
		return false;

	// Check (square) distance
	if (dot(_pos - vec2(x, y), _pos - vec2(x, y)) <= _radius * _radius)
		return true;
	else
		return false;

}

float Ball::touchingBallDistance(const Ball & other) {
	float dist;

	// first, filter far away balls
	if ((_pos.x - _radius > other._pos.x + other._radius) |
		(_pos.x + _radius < other._pos.x - other._radius) ||
		(_pos.y - _radius > other._pos.y + other._radius) ||
		(_pos.y + _radius < other._pos.y - other._radius))
		return -1.f;

	dist = glm::length(other._pos - _pos);
	if (dist <= _radius + other._radius)		// balls touching
		return dist;
	else
		return -1.f;

}
