#include "Ball.h"


Ball::Ball(float x, float y) {
	_pos.x = x;
	_pos.y = y;
}

inline bool Ball::onFloor() {
	//return _y - _cur_radius <= -1;
	return _pos.y - _cur_radius <= -1;
}

inline bool Ball::onRightWall() {
	//return _x + _cur_radius >= 1;
	return _pos.x + _cur_radius >= 1;
}

inline bool Ball::onLeftWall() {
	//return _x - _cur_radius <= -1;;
	return _pos.x - _cur_radius <= -1;
}

inline void  Ball::wallCollison()
{
	// side walls_velo. 
	if ((onLeftWall() && _velo.x < 0) || (onRightWall() && _velo.x > 0))
		_velo.x *= -WALL_COLLISION_DECELERATION;

	// floor
	if (onFloor() && _velo.y != 0) {
		if (_velo.y < 0)
			_velo.y *= -FLOOR_COLLISION_DECELERATION;
		else if (_velo.y < GRAVITY_PER_FRAME) {
			_pos.y = _cur_radius - 1;
			_velo.y = 0;
		}
	}
}

void Ball::ballCollision(Circle& other)
{
	glm::vec2 axis;
	float dist, axis_speed, other_axis_speed, delta_speed;


	if ((_pos.x - _cur_radius <= other._pos.x + other._cur_radius) &&
		(_pos.x + _cur_radius >= other._pos.x - other._cur_radius) &&
		(_pos.y - _cur_radius <= other._pos.y + other._cur_radius) &&
		(_pos.y + _cur_radius >= other._pos.y - other._cur_radius))
		// filtering far away balls
	{
		dist = glm::length(other._pos - _pos);
		if (dist <= _cur_radius + other._cur_radius && dist > 0.f){		// balls touching
			axis = glm::normalize((other._pos - _pos));
			axis_speed = glm::dot(_velo, axis);
			other_axis_speed = glm::dot(other._velo, axis);

			if ((delta_speed = axis_speed - other_axis_speed) > 0) {	// going at eachother
				_velo -= axis * delta_speed * BALL_COLLISION_DECELERATION;
				other._velo += axis * delta_speed * BALL_COLLISION_DECELERATION;
			}
		}
	}


}
