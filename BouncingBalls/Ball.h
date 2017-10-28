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

	/*inline bool touchingLeftWall() {
		return _pos.x - _radius <= -1;
	}*/

	inline bool touchingLeftWall(float leftWallPos) {
		return _pos.x - _radius <= leftWallPos;
	}

	/*inline bool touchingRightWall() {
		return _pos.x + _radius >= 1;
	}*/

	inline bool touchingRightWall(float rightWallPos) {
		return _pos.x + _radius >= rightWallPos;
	}

	/*inline bool touchingFloor() {
		return _pos.y - _radius <= -1.f;
	}*/

	inline bool touchingFloor(float floorPos) {
		return _pos.y - _radius <= floorPos;
	}

	void ballCollision(Ball& other);
	
	bool containsPoint(float x, float y);

	bool fitToScreen(float wallPos, float ceilingPos) {
		bool inBounds = true;

		if (touchingLeftWall(-wallPos)) {
			_pos.x = -wallPos + _radius;
			inBounds = false;
		}

		else if (touchingRightWall(wallPos)) {
			_pos.x = wallPos - _radius;
			inBounds = false;
		}

		if (touchingFloor(-ceilingPos)) {
			_pos.y = -ceilingPos + _radius;
			inBounds  = false;
		}


		return inBounds;

	}

	float touchingBallDistance(const Ball & other);
};


#endif	// BALL_H