#ifndef BALL_H
#define BALL_H

#include <glm/vec2.hpp>		// glm::vec2 (pos and velo)

struct Ball
{
	glm::vec2 _pos, _velo;
	const float* _color;
	float _def_radius, _cur_radius;

	Ball(float x = 0.f, float y = 0.f);

	bool onFloor();

	bool onLeftWall();

	bool onRightWall();

	void wallCollison();

	void ballCollision(Ball& other);

};







#endif	// BALL_H