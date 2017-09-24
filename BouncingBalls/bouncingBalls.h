#ifndef BOUNCING_BALLS_H
#define BOUNCING_BALLS_H


#include "Ball.h"				// struct Ball;
#include <ogldev_callbacks.h>	// ICallbacks;


#include <GL/glew.h>			// typedef GLuint;
#include <glm/vec2.hpp>			// glm::vec2;
#include <vector>				// holding the balls


class BouncingBalls : public ICallbacks {

public:

	BouncingBalls();

	virtual ~BouncingBalls();

	bool Init();

	void Run();

	

	// ICallbacks overriding
	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState);
	virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y);
	virtual void RenderSceneCB();
	virtual void TimerCB(int value);
	


private:

	// Vector storing the balls
	std::vector<Ball> _balls;

	// Window sizes:
	float _width, _height, _offsetX, _offsetY;

	glm::vec2 _lightPos;

	GLuint _vbo;
	GLint _posAttrib;
	GLuint _fillColorUV;
	GLuint _centerUV;		// Uniform handle for center variable
	GLuint _radiusUV;		// Uniform handle for radius of the ball
	GLuint _lightUV;		// Uniform handle for light on the ball; 
	bool _animate;

	
	void addBall(float x = 0, float y = 0);

	void handleBallsCollisions();

	void moveBalls();

	void handleWallsCollisions();

	void drawBalls();


};

#endif //BOUNCING_BALLS_H
