// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Lib includes
#define _USE_MATH_DEFINES
#include "bouncingBalls.h"
#include "ogldev_glut_backend.h"		// glut backend calls
#include "ogldev_util.h"				// #define INVALID_UNIFORM_LOCATION;
#include "ShaderIO.h"					// handling shaders

// math includes
#include "glm/geometric.hpp"	// normalize(); length();
#include <algorithm>			// min(); max();



#define VERTEX_SHADER_PATH "SimpleShader.vert"
#define FRAGMENT_SHADER_PATH "SimpleShader.frag"
//#define PI 3.1415926f


/************************************************************/
//				 STATIC IMPLEMENTATION SECTION				//
/************************************************************/

#define BASE_WINDOW_SIZE 768

#define CIRCLE_EDGES_AMOUNT 30
#define SCALARS_PER_VERTEX 2
#define DFLT_RADIUS 0.15f
#define BOUNCINESS 0.85f
#define GRAVITY_POWER  (1.0f / (1 << 17))

#define INITIAL_FRAME_RATE_MILIS 20

#define LIGHT_POS_X 1.5f
#define LIGHT_POS_Y -2.0f


/************************************************************/
//							GLOBALS							//
/************************************************************/

static glm::ivec2 gMousePos;		// For mouse's active motion
static int gDeltaTime;			// For balls' movements
static int gScreenWidth;			// For mouse's active motion (drop)
static int gScreenHeight;
typedef BouncingBalls BBs;

static enum ACTIVE_MOUSE_STATUS { ACTIVE_MOUSE_ON, IDLE_FUNCTION_READ_ACTIVE_MOUSE_ON, ACTIVE_MOUSE_OFF };

static int gTime;
static int gMouseActiveMotionTime;  // see note about tracking the speed of held ball;
static glm::vec2 gHeldBallBigDelta;
static ACTIVE_MOUSE_STATUS gActiveMouseStatus;






/************************************************************/
//	Note about tracking speed of held ball:
// appearently, even when dragging the ball continously with the mouse, the 
// mouse active motion isn't called every loop iteration. 
// Hence, to track speed of mouse dragging, one should keep track of last time the mouse active motion
// function was called - 'gActiveMotionTime', which again, is different then the gTime, since again,
// the active mouse call back isn't called every loop iteration.
/************************************************************/



/************************************************************/
//					HELPER FUNCTIONS						//
/************************************************************/

static float rand_float(size_t log_n) {
	int n = 1 << log_n;
	int m = std::rand() & (2 * n - 1);
	return (float)(m - n) / (float)n;
}

static const float* pickAColor()
{
	// Colors:
	const static float RED[4] = { 1.f, 0.f, 0.f, 1.f };
	const static float GREEN[4] = { 0.f, 1.f, 0.f, 1.f };
	const static float BLUE[4] = { 0.f, 0.f, 1.f, 1.f };
	const static float YELLOW[4] = { 1.f, 1.f, 0.f, 1.f };
	const static float NAVY[4] = { 0.f, 0.f, 128.f / 255, 1.f };
	const static float LAWN_GREEN[4] = { 124.f / 252, 252.f / 255, 0.f, 1.f };


	//const static float* COLORS_ARRAY[] = { RED, BLUE, YELLOW, GREEN,  NAVY };
	const static float* COLORS_ARRAY[] = { RED, BLUE, YELLOW, GREEN};

	return COLORS_ARRAY[std::rand() % (sizeof(COLORS_ARRAY) / sizeof(float *))];

}


/************************************************************/
//				BOUNCING BALLS IMPLEMENTATION				//
/************************************************************/

BBs::BouncingBalls(){
	
	// Screen dimensions
	
	/*_wallPos = 1.f;
	_ceilingPos = 1.f;*/
	_ratioWorldToScreen = 2.f / BASE_WINDOW_SIZE;
	gScreenWidth = BASE_WINDOW_SIZE;
	gScreenHeight = BASE_WINDOW_SIZE;

	////////////////////////
	_worldXRadius = 1.f;
	_worldYRadius = 1.f;
	////////////////////////

	// Animation on
	_animate = true;

	// Light positions
	_lightPos.x = LIGHT_POS_X;
	_lightPos.y = LIGHT_POS_Y;

	// Held Ball
	it_heldBall = _balls.end();
}

BBs::~BouncingBalls()
{
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

BBs & BouncingBalls::instance() {
	static BouncingBalls Instance;
	return Instance;
}

bool BBs::Init(int argc, char ** argv)
{
	
	// Initialize GLUT Backend
	GLUTBackendInit(argc, argv, false, false, true);

	// Create window & init glew
	if (!GLUTBackendCreateWindow(BASE_WINDOW_SIZE, BASE_WINDOW_SIZE, false, "Bouncing Balls"))
		return false;



	// Create the shader program
	GLuint program = programManager::sharedInstance().createProgram
					("default",
					VERTEX_SHADER_PATH,
					FRAGMENT_SHADER_PATH);
	glUseProgram(program);

	// Uniform Variables' locations
	_ballCenterUVLoc = glGetUniformLocation(program, "ballCenter");
	_ballRadiusUVLoc = glGetUniformLocation(program, "ballRadius");
	_ballColorUVLoc  = glGetUniformLocation(program, "ballColor");
	_lightHitUVLoc = glGetUniformLocation(program, "lightHit");
	/*_wallPosUVLoc = glGetUniformLocation(program, "wallPos");
	_ceilingPosUVLoc = glGetUniformLocation(program, "ceilingPos");*/
	
	////////////////////////
	_worldXRadiusUVLoc = glGetUniformLocation(program, "worldXRadius");
	_worldYRadiusUVLoc = glGetUniformLocation(program, "worldYRadius");
	////////////////////////

	// Check valid locations
	/*if (_ballCenterUVLoc == INVALID_UNIFORM_LOCATION ||
		_ballRadiusUVLoc == INVALID_UNIFORM_LOCATION ||
		_ballColorUVLoc == INVALID_UNIFORM_LOCATION ||
		_lightLoc == INVALID_UNIFORM_LOCATION ||
		_relativeWidthLoc == INVALID_UNIFORM_LOCATION ||
		_relativeHeightLoc == INVALID_UNIFORM_LOCATION || 
		_wallPosLoc == INVALID_UNIFORM_LOCATION || 
		_ceilingPosLoc == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Unable to get the location of one of the uniform vars\n");
		return false;
	}*/
	
	
	/*** Create the vertices of the prototype ball ***/
		
	float vertices[CIRCLE_EDGES_AMOUNT * SCALARS_PER_VERTEX];
		
	int vertex = 0;
	float angle = 0;
	for (vertex = 0; vertex < CIRCLE_EDGES_AMOUNT; vertex++, angle += 2.f * M_PI / CIRCLE_EDGES_AMOUNT){
		vertices[SCALARS_PER_VERTEX * vertex] = std::cos(-angle);		// x coordinate
		vertices[SCALARS_PER_VERTEX * vertex + 1] = std::sin(-angle);	// y coordinate
		/* Note: The negative angle is so we have clock-wise vertices */
	}

	
	// Create and load Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		

	// set array interpretation
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,			// pos attribute handle
							SCALARS_PER_VERTEX,	// number of scalars per vertex
							GL_FLOAT,				// scalar type
							GL_FALSE,
							0,
							0);
		


	// Set time
	gTime = glutGet(GLUT_ELAPSED_TIME);

	return true;
}

void BBs::Run() {
	GLUTBackendRun(this);
}

BBs::BallIter BouncingBalls::addBall(Ball & newBall)
{
	//Ball ball = Ball(x, y, DFLT_RADIUS);
	newBall._bounciness = BOUNCINESS;

	// Pick color
	newBall._color = pickAColor();

	
	
	//newBall.fitToScreen(_wallPos, _ceilingPos);
	newBall.fitToScreen(_worldXRadius, _worldYRadius);		// make 'fitToScreen' not a Ball method

	


	return _balls.insert(_balls.end(), newBall);
}

inline void BBs::handleBallsCollisions()
{
	std::list<Ball>::iterator tmp;
	for (auto it1 = _balls.begin(); it1 != _balls.end(); it1++) {
		tmp = it1;
		for (auto it2 = tmp++; it2 != _balls.end(); it2++)
			it1->ballCollision(*it2);
	}

}

void BBs::moveBalls()
{
	float BOUNCE_THRESHOLD = gDeltaTime * GRAVITY_POWER;
	float tmp;


	// move each ball
	for (auto it = _balls.begin(); it != _balls.end(); it++) {

		if (it == it_heldBall)
			continue;

		// Horizintal movement + Walls collision
		it->_pos.x += gDeltaTime * (tmp = it->_velo.x);		
		if ((it->touchingLeftWall(-_worldXRadius) && tmp < 0) ||
			(it->touchingRightWall(_worldXRadius) && tmp > 0))
			it->_velo.x *= -it->_bounciness;

		// Vertical movement 
		it->_pos.y += gDeltaTime * it->_velo.y;


		// GRAVITY & FLOOR COLLISION :

		// "touching" floor, several cases:
		if (it->touchingFloor(-_worldYRadius)) {

			// resting ball (vertically) stays resting :
			if (it->_velo.y == 0.f)
				//it->_pos.y = -_ceilingPos + it->_radius;
				it->_pos.y = -_worldYRadius + it->_radius;


			// too slow - "rest" the ball
			else if (it->_velo.y < BOUNCE_THRESHOLD && it->_velo.y > -BOUNCE_THRESHOLD) {
				//it->_pos.y = -_ceilingPos + it->_radius;
				it->_pos.y = -_worldYRadius + it->_radius;
				it->_velo.y = 0.f;
			}

			// not too slow up - apply gravity
			else if (it->_velo.y > 0) {
				it->_velo.y -= gDeltaTime * GRAVITY_POWER;
			}

			// going down inside floor -  don't apply gravity, bounce back off floor
			else {
				it->_velo.y *= -it->_bounciness;
			}
		}

		// not touching floor - apply gravity
		else
			it->_velo.y -= gDeltaTime * GRAVITY_POWER;


	}	// Gravity and Floor

}

void BBs::drawBalls()
{
	glm::vec2 lightHit;
	float distFromLight;
	for (auto it = _balls.begin(); it != _balls.end(); ++it){

		// lighting, extract the light hit pos on the ball
		distFromLight = glm::length(it->_pos - _lightPos);
		lightHit = it->_pos + (_lightPos - it->_pos) * (it->_radius / distFromLight) * 0.33f;
		
		// Unifor Variables of Current Ball
		glUniform2f(_ballCenterUVLoc, it->_pos.x, it->_pos.y);
		glUniform1f(_ballRadiusUVLoc, it->_radius);
		glUniform4fv(_ballColorUVLoc, 1, it->_color);
		glUniform1f(_worldXRadiusUVLoc, _worldXRadius);
		glUniform1f(_worldYRadiusUVLoc, _worldYRadius);
		glUniform2fv(_lightHitUVLoc, 1, (GLfloat *)&lightHit);
		
		// Draw ball
		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_EDGES_AMOUNT + 2);

	}
}

void BBs::ReshapeCB(int width, int height) {
	int t1 = glutGet(GLUT_ELAPSED_TIME);
	
	gScreenWidth = width;
	gScreenHeight = height;

	/*_wallPos = (float)(width) / BASE_WINDOW_SIZE;
	_ceilingPos = (float)(height) / BASE_WINDOW_SIZE;*/

	
	_worldXRadius = (float)(width) / BASE_WINDOW_SIZE;
	_worldYRadius = (float)(height) / BASE_WINDOW_SIZE;
	

	glViewport(0, 0, width, height);

	

}

void BBs::KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState) {

	switch (OgldevKey) {

	case OGLDEV_KEY_q:
		GLUTBackendLeaveMainLoop();
		break;

	case OGLDEV_KEY_k:
		_animate = !_animate;
		break;

	}
}

void BBs::MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y) {
	
	// Only left clicks
	if (Button == OGLDEV_MOUSE_BUTTON_RIGHT)
		return;

	// MOUSE PRESS
	if (State == OGLDEV_KEY_STATE_PRESS) {	

		assert(it_heldBall == _balls.end());

		glm::vec2 worldPos = screenToWorldCords(x, y);
		Ball newBall;
		BallIter it;

		

		// Check if clicked on existing ball
		for (it = _balls.begin(); it != _balls.end(); ++it)
			if (it->containsPoint(worldPos.x, worldPos.y))
				break;

		if (it != _balls.end())
			it_heldBall = it;
		else {
			newBall = Ball(worldPos.x, worldPos.y, DFLT_RADIUS);
			it_heldBall = addBall(newBall);
			GLUTBeckendPostRedisplay();
		}

		it_heldBall->_static = true;

		// Update mouse pos, time, and delta.
		gMousePos = glm::ivec2(x, y);
		gHeldBallBigDelta = glm::vec2();
		gMouseActiveMotionTime = gTime;
		
	}

	// MOUSE RELEASE:
	else {
		// If still holding ball (can be dropped during mouse motion out of bounds)
		if (it_heldBall != _balls.end()) {


			if (gActiveMouseStatus == ACTIVE_MOUSE_OFF) {
				it_heldBall->_velo.x = 0.0;
				it_heldBall->_velo.y = 0.0;
			}
			


			it_heldBall->_static = false;
			it_heldBall = _balls.end();
		}

		gActiveMouseStatus = ACTIVE_MOUSE_OFF;

	}
		

	

	
}

void BBs::RenderSceneCB() {

	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//GLenum polygonMode = GL_POINT;   // Also try using GL_FILL and GL_POINT
	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);


	// Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// Move and Draw balls
	moveBalls();
	handleBallsCollisions();
	drawBalls();
	GLUTBackendSwapBuffers();
	
}

void BBs::IdleCB() {

	gDeltaTime = glutGet(GLUT_ELAPSED_TIME) - gTime;
	gTime += gDeltaTime;
	
	
	if (_animate && gDeltaTime > 0)
		GLUTBeckendPostRedisplay();
	

	if (gActiveMouseStatus == ACTIVE_MOUSE_ON)
		gActiveMouseStatus = IDLE_FUNCTION_READ_ACTIVE_MOUSE_ON;
	else
		gActiveMouseStatus = ACTIVE_MOUSE_OFF;
}

void BBs::MouseActiveMotionCB(int x, int y) {
	
	assert(gActiveMouseStatus != ACTIVE_MOUSE_ON);

	gActiveMouseStatus = ACTIVE_MOUSE_ON;

	// if not holding ball, return.
	if (it_heldBall == _balls.end())
		return;
	
	
	glm::vec2 smallDelta = screenToWorldDelta(x - gMousePos.x, y - gMousePos.y);
	gHeldBallBigDelta += smallDelta;



	// Move the ball with the mouse
	it_heldBall->_pos += smallDelta;
	
	// Set speed
	int macDeltaTime; // mouseActiveMotionDeltaTime
	//if (gDeltaTime > 0) {
	if ((macDeltaTime = gTime - gMouseActiveMotionTime) > 0) {
		
		it_heldBall->_velo = gHeldBallBigDelta / (float)(macDeltaTime);
		//it_heldBall->_velo = smallDelta / (float)(gDeltaTime);

		gHeldBallBigDelta = glm::vec2();
	}


	// If mouse out of screen - drop the ball
	if (!it_heldBall->fitToScreen(_worldXRadius, _worldYRadius)
		&& (x < 0 || x > gScreenWidth || y < 0 || y > gScreenHeight)) {
		
		it_heldBall->_static = false;
		it_heldBall = _balls.end();
		
	}
	
	// Update mouse pos and time
	gMousePos = glm::ivec2(x, y);
	gMouseActiveMotionTime = gTime;
}


