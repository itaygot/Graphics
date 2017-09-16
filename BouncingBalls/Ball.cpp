#include "Ball.h"
#include "glm/geometric.hpp"	// normalize(); length(); dot();



Ball::Ball(float x, float y) {
	_pos.x = x;
	_pos.y = y;
	_bounciness = 1.f;
}

bool Ball::insideFloor() {
	return _pos.y - _radius <= -1;
}

bool Ball::onRightWall() {
	//return _x + _radius >= 1;
	return _pos.x + _radius >= 1;
}

bool Ball::onLeftWall() {
	//return _x - _radius <= -1;;
	return _pos.x - _radius <= -1;
}

void  Ball::wallCollison()
{
	// side walls_velo. 
	if ((onLeftWall() && _velo.x < 0) || (onRightWall() && _velo.x > 0))
		//_velo.x *= -WALL_COLLISION_DECELERATION;
		_velo.x *= -_bounciness;
	
	// floor
	//if (onFloor() && _velo.y != 0) {
	//	if (_velo.y < 0)
	//		//_velo.y *= -FLOOR_COLLISION_DECELERATION;
	//		_velo.y *= -_bounciness;
	//	else if (_velo.y < GRAVITY_PER_FRAME) {
	//		_pos.y = _radius - 1;
	//		_velo.y = 0;
	//	}
	//}

	if (insideFloor() && _velo.y < 0) {
		_velo.y *= -_bounciness;
	}
}

void Ball::ballCollision(Ball& other)
{
	glm::vec2 axis;
	//float dist, axis_speed, other_axis_speed, delta_speed;
	float dist, axis_speed;

	// first, filter far away balls
	if ((_pos.x - _radius <= other._pos.x + other._radius) &&
		(_pos.x + _radius >= other._pos.x - other._radius) &&
		(_pos.y - _radius <= other._pos.y + other._radius) &&
		(_pos.y + _radius >= other._pos.y - other._radius))
	{
		dist = glm::length(other._pos - _pos);
		if (dist <= _radius + other._radius && dist > 0.f){		// balls touching
			axis = glm::normalize((other._pos - _pos));
			axis_speed = glm::dot(_velo, axis) - glm::dot(other._velo, axis);
			//other_axis_speed = glm::dot(other._velo, axis);

			//if ((delta_speed = axis_speed - other_axis_speed) > 0) {	// going at eachother
			if (axis_speed > 0) {	// going at eachother
				_velo -= axis * axis_speed * _bounciness;
				//other._velo += axis * delta_speed * BALL_COLLISION_DECELERATION;
				other._velo += axis * axis_speed * other._bounciness;
			}
		}
	}


}
