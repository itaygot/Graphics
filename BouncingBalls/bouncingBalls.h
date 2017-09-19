#ifndef BOUNCING_BALLS_H
#define BOUNCING_BALLS_H

#include <GL/glew.h>		// typedef GLuint;
#include "Ball.h"			// Ball;
#include <vector>			// holding the balls
#include <ogldev_callbacks.h>


class BouncingBalls : public ICallbacks {

public:

	BouncingBalls();

	virtual ~BouncingBalls();

	void init();

	void draw();

	void resize(int width, int height);

	
	void addBall(float x = 0, float y = 0);

	// ICallbacks
	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState);

private:

	// Vector storing the balls
	std::vector<Ball> _balls;

	// Window sizes:
	float _width, _height, _offsetX, _offsetY;

	// Light source uniform locations
	GLfloat _lightSource[2];

	GLuint _vbo;
	GLint _posAttrib;
	GLuint _fillColorUV;
	GLuint _centerUV;		// Uniform handle for center variable
	GLuint _radiusUV;		// Uniform handle for radius of the ball
	GLuint _lightUV;		// Uniform handle for light on the ball; 
	bool _animate;

	


	inline void handleBallsCollisions();

	inline void moveBalls();

	inline void handleWallsCollisions();

	inline void drawBalls();


};

#endif //BOUNCING_BALLS_H
