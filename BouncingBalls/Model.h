//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <GL/glew.h>		// typedef GLuint;
#include "Ball.h"			// Ball;
#include <vector>			// holding the balls


class Model {

public:

	Model();

	virtual ~Model();

	void init();

	void draw();

	void resize(int width, int height);

	
	void addBall(float x = 0, float y = 0);


private:
	GLuint _vbo;
	GLint _posAttrib;
	GLuint _fillColorUV;
	GLuint _centerUV;		// Uniform handle for center variable
	GLuint _radiusUV;		// Uniform handle for radius of the ball
	GLuint _lightUV;		// Uniform handle for light on the ball; 

	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	// Vector storing the balls
	std::vector<Ball> _balls;

	// Light source location
	GLfloat _lightSource[2];


	inline void handleBallsCollisions();

	inline void moveBalls();

	inline void handleWallsCollisions();

	inline void drawBalls();

};

#endif /* __ex0__Model__ */
