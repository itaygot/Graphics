#ifndef BALL_H
#define BALL_H

#include <glm/vec2.hpp>		// glm::vec2 (pos and velo)

struct Ball
{
	glm::vec2 _pos, _velo;
	const float* _color;
	float _radius;
	float _bounciness;
	bool _static;

	Ball(float x = 0.f, float y = 0.f);

	Ball(float x, float y, float radius);

	inline bool touchingLeftWall() {
		return _pos.x - _radius <= -1;
	}

	inline bool touchingRightWall() {
		return _pos.x + _radius >= 1;
	}

	inline bool touchingFloor() {
		return _pos.y - _radius <= -1.f;
	}

	float touchingBall(const Ball & other);

	void ballCollision(Ball& other);

	inline bool fitToScreen() {
		bool inBounds = true;

		if (touchingLeftWall()) {
			_pos.x = -1.f + _radius;
			inBounds = false;
		}

		else if (touchingRightWall()) {
			_pos.x = 1.f - _radius;
			inBounds = false;
		}

		if (touchingFloor()) {
			_pos.y = -1.f + _radius;
			inBounds  = false;
		}

		return inBounds;

	}




};


#endif	// BALL_H