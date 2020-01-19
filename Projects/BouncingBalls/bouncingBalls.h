#ifndef BOUNCING_BALLS_H
#define BOUNCING_BALLS_H



#include "Ball.h"				// struct Ball;
#include <ogldev_callbacks.h>	// ICallbacks;
#include <GL/glew.h>			// typedef GLuint;
#include <glm/vec2.hpp>		// glm::vec2;

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

	
	std::list<Ball> _balls;				// List holding the balls
	BallIter	it_heldBall;			// Held ball
	glm::vec2	_lightPos;				// Light positions
	GLuint		_vbo;
	GLuint		_ballCenterUVLoc;	// Location of the Uniform variable in the shader program
	GLuint		_ballRadiusUVLoc;	// Location of the Uniform variable
	GLuint		_ballColorUVLoc;	// Location of the Uniform variable in the shader program
	GLuint		_lightHitUVLoc;		// Location of the Uniform variable
	bool		_animate;
	

	//float		_wallPos;					// EXPLENATION NEEDED !!!
	//float		_ceilingPos;				// EXPLENATION NEEDED !!!
	float		_ratioWorldToScreen;
	//GLuint		_wallPosUVLoc;				// EXPLENATION NEEDED !!!
	//GLuint		_ceilingPosUVLoc;


	float		_worldXRadius;					// EXPLENATION NEEDED !!!
	float		_worldYRadius;					// EXPLENATION NEEDED !!!
	GLuint		_worldXRadiusUVLoc;
	GLuint		_worldYRadiusUVLoc;




	/*float		_leftWallPos;
	float		_rightWallPos;
	float		_floorPos;
	float		_ceilingPos2;
	GLuint		_frameCenterUVLocation;
	GLuint		_frameDimsUVLocation;
*/

	
	BouncingBalls();			// Private C'tor (singleton class)
	virtual ~BouncingBalls();	// Private D'tor (not sure if must be private for singleton)

	//BallIter addBall(float x, float y);
	BallIter addBall(Ball & newBall);
	void handleBallsCollisions();
	void moveBalls();
	void drawBalls();
	
	

	inline glm::vec2 screenToWorldCords(int x, int y) {
		return glm::vec2(
			-_worldXRadius + x * _ratioWorldToScreen, 
			_worldYRadius - y * _ratioWorldToScreen);
	}

	inline glm::vec2 screenToWorldDelta(int x, int y) {
		return glm::vec2(x * _ratioWorldToScreen, -y * _ratioWorldToScreen);
	}


};

#endif //BOUNCING_BALLS_H
