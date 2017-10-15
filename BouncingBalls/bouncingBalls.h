#ifndef BOUNCING_BALLS_H
#define BOUNCING_BALLS_H


#include "Ball.h"				// struct Ball;
#include <ogldev_callbacks.h>	// ICallbacks;


#include <GL/glew.h>			// typedef GLuint;
#include <glm/vec2.hpp>			// glm::vec2;
//#include <vector>				// holding the balls
#include <list>				// holding the balls


class BouncingBalls : public ICallbacks {

public:
	typedef std::list<Ball>::iterator BallIter;
	static BouncingBalls & instance();

	bool Init(int argc, char ** argv);

	void Run();

	// Delete copy c'tor and assignment operator (singleton class)
	BouncingBalls(const BouncingBalls &) = delete;
	BouncingBalls operator=(const BouncingBalls &) = delete;
	

	// ICallbacks overriding
	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState);
	virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y);
	virtual void RenderSceneCB();
	virtual void IdleCB();
	virtual void MouseActiveMotionCB(int x, int y);
	virtual void ReshapeCB(int width, int height);


private:

	
	std::list<Ball> _balls;		// List holding the balls
	BallIter	it_heldBall;		// Held ball
	glm::vec2	_lightPos;		// Light positions
	GLuint		_vbo;
	GLint		_posAttrib;
	GLuint		_fillColorUV;
	GLuint		_centerUV;		// Uniform handle for center variable
	GLuint		_radiusUV;		// Uniform handle for radius of the ball
	GLuint		_lightUV;		// Uniform handle for light on the ball;
	int			_windowWidth;
	int			_windowHeight;
	bool		_animate;

	
	BouncingBalls();			// Private C'tor (singleton class)
	virtual ~BouncingBalls();	// Private D'tor (not sure if must be private for singleton)

	BallIter addBall(float x, float y);
	void handleBallsCollisions();
	void moveBalls();
	void drawBalls();
	BallIter checkForBall(float x, float y, float radius);

	inline glm::vec2 screenToWorldCords(int x, int y) {
		return glm::vec2(
			(float)x * 2 / _windowWidth - 1, 1 - (float)y * 2 / _windowHeight);
	}

	inline glm::vec2 screenToWorldDelta(int x, int y) {
		return glm::vec2(
			(float)x * 2 / _windowWidth, -(float)y * 2 / _windowHeight);
	}

};

#endif //BOUNCING_BALLS_H
