// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Lib includes
#include "bouncingBalls.h"
#include "ogldev_glut_backend.h"		// glut backend calls
#include "ShaderIO.h"					// handling shaders

// math includes
#include <cmath>				// std::sin(); std::cos(); std::rand();
#include "glm/geometric.hpp"	// normalize(); length();
#include <algorithm>			// min(); max();



#define VERTEX_SHADER_PATH "SimpleShader.vert"
#define FRAGMENT_SHADER_PATH "SimpleShader.frag"
#define PI 3.1415926f


/************************************************************/
//				 STATIC IMPLEMENTATION SECTION				//
/************************************************************/

#define INITIAL_WINDOW_WIDTH 600
#define INITIAL_WINDOW_HEIGHT 600

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

static float gDeltaTime;
static glm::ivec2 gMousePos;
typedef BouncingBalls BBs;

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


	const static float* COLORS_ARRAY[] = { RED, BLUE, YELLOW, GREEN, LAWN_GREEN, NAVY };

	return COLORS_ARRAY[std::rand() % (sizeof(COLORS_ARRAY) / sizeof(float *))];

}

static inline glm::vec2 screenToWorldCoords(int x, int y) {
	return glm::vec2((float)x * 2 / glutGet(GLUT_WINDOW_WIDTH) - 1,
		1 - (float)y * 2 / glutGet(GLUT_WINDOW_HEIGHT));
}

static inline glm::vec2 screenToWorldDelta(int x, int y) {
	return glm::vec2((float)x * 2 / glutGet(GLUT_WINDOW_WIDTH),
		-(float)y * 2 / glutGet(GLUT_WINDOW_HEIGHT));
}

/************************************************************/
//				BOUNCING BALLS IMPLEMENTATION				//
/************************************************************/

BBs::BouncingBalls(){
	
	// Screen dimensions
	_windowWidth = INITIAL_WINDOW_WIDTH;
	_windowHeight = INITIAL_WINDOW_HEIGHT;

	// Animation on
	_animate = true;

	// Light positions
	_lightPos[0] = LIGHT_POS_X;
	_lightPos[1] = LIGHT_POS_Y;

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
	
	// Initialize GLUT 
	GLUTBackendInit(argc, argv, false, false);

	// Create window & init glew
	if (!GLUTBackendCreateWindow(INITIAL_WINDOW_HEIGHT, INITIAL_WINDOW_WIDTH, false, "Bouncing Balls"))
		return false;

	// Create the shader program
	GLuint program = programManager::sharedInstance().createProgram
					("default",
					VERTEX_SHADER_PATH,
					FRAGMENT_SHADER_PATH);
	glUseProgram(program);

	// Uniform Variables
	_fillColorUV  = glGetUniformLocation(program, "fillColor");
	_centerUV = glGetUniformLocation(program, "center");
	_radiusUV = glGetUniformLocation(program, "radius");
	_lightUV = glGetUniformLocation(program, "light");

	
	/*** Create the vertices of the prototype ball ***/
		
	float vertices[CIRCLE_EDGES_AMOUNT * SCALARS_PER_VERTEX];
		
	int vertex = 0;
	float angle = 0;
	for (vertex = 0; vertex < CIRCLE_EDGES_AMOUNT; vertex++, angle += 2.f * PI / CIRCLE_EDGES_AMOUNT){
		vertices[SCALARS_PER_VERTEX * vertex] = std::cos(-angle);		// x coordinate
		vertices[SCALARS_PER_VERTEX * vertex + 1] = std::sin(-angle);	// y coordinate
		/* Note: The negative angle is so we have clock-wise vertices */
	}

	
	// Create and load Vertex Buffer Object:
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		

	// set array interpretation
	_posAttrib = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(_posAttrib);
	glVertexAttribPointer(_posAttrib,			// attribute handle
							SCALARS_PER_VERTEX,	// number of scalars per vertex
							GL_FLOAT,				// scalar type
							GL_FALSE,
							0,
							0);
		


	

	return true;
}

void BBs::Run() {
	GLUTBackendRun(this);
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

		// Check if clicked on ball
		glm::vec2 worldPos = screenToWorldCoords(x, y);
		BallIter it = checkForBall(worldPos.x, worldPos.y, DFLT_RADIUS);

		if (it != _balls.end())
			it_heldBall = it;
		else
			it_heldBall = addBall(worldPos.x, worldPos.y);

		it_heldBall->_static = true;

		// Update mouse pos
		gMousePos = glm::ivec2(x, y);
		
	}

	// MOUSE RELEASE:
	else
		// If still holding ball (can be dropped during mouse motion out of bounds)
		if (it_heldBall != _balls.end()) {
			it_heldBall->_static = false;
			it_heldBall = _balls.end();
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

	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int timeNow = glutGet(GLUT_ELAPSED_TIME);

	gDeltaTime = (float)(timeNow - lastTime);
	lastTime = timeNow;
	

//	// variables (and their initializations)
//	float dt;
//#ifdef _WIN32
//	static DWORD last_time;
//	DWORD time_now = GetTickCount();
//#else
//	struct timeval time_now;
//	gettimeofday(&time_now, NULL);
//	static struct timeval last_time;
//#endif
//	static bool inited = false;
//
//
//	// only at first call
//	if (!inited) {
//		gIdleElapsedMilis = INITIAL_FRAME_RATE_MILIS;
//		inited = true;
//	}
//
//	else {
//#ifdef _WIN32
//		gIdleElapsedMilis = (float)(time_now - last_time);
//		
//#else
//		gettimeofday(&time_now, NULL);
//		gIdleElapsedMilis = 1.0e+3 * (float)(time_now.tv_sev - last_time.tv_sec) +
//			1.0e-3* (float)(time_now.tv_usec - last_time.tv_usec);
//#endif
//
//	}
//
//	last_time = time_now;


	if (_animate)
		GLUTBeckendPostRedisplay();
	
}

void BBs::MouseActiveMotionCB(int x, int y) {

	// if not holding ball, return.
	if (it_heldBall == _balls.end())
		return;
	
	glm::vec2 delta = screenToWorldDelta(x - gMousePos.x, y - gMousePos.y);

	// Move the ball with the mouse
	it_heldBall->_pos += delta;
	
	// Set speed
	it_heldBall->_velo = delta / (2.f * gDeltaTime);

	// If hitting walls - drop the ball and set its new speed
	if(!it_heldBall->fitToScreen()){
		

		if()

		it_heldBall->_static = false;
		it_heldBall = _balls.end();
	}

	// Update mouse pos
	gMousePos = glm::ivec2(x, y);

}

BBs::BallIter BBs::checkForBall(float x, float y, float radius) {
	Ball ball(x, y, radius);
	BallIter it;

	for (it = _balls.begin(); it != _balls.end(); ++it)
		if (ball.touchingBall(*it) >= 0) {
			return it;
		}
	return _balls.end();
}

BBs::BallIter BouncingBalls::addBall(float x, float y)
{
	Ball ball = Ball(x, y, DFLT_RADIUS);
	ball._bounciness = BOUNCINESS;

	// Pick color
	ball._color = pickAColor();

	// Draw a velocity
	ball._velo.x = rand_float(3) * 0.1f / 32;
	ball._velo.y = rand_float(3) * 0.1f / 32;
	/*ball._velo.x  = 0;
	ball._velo.y = 0;*/

	ball.fitToScreen();

	
	return _balls.insert(_balls.end(), ball);
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
	//float BOUNCE_THRESHOLD = GRAVITY_POWER;	
	float tmp;
	

	// move each ball
	for (auto it = _balls.begin(); it != _balls.end(); it++) {

		if (it == it_heldBall)
			continue;

		// Horizintal movement + Walls collision
		it->_pos.x += gDeltaTime * (tmp = it->_velo.x);
		if ((it->touchingLeftWall() && tmp < 0) ||
			(it->touchingRightWall() && tmp > 0))
			it->_velo.x *= -it->_bounciness;



		// Vertical movement 
		it->_pos.y += gDeltaTime * it->_velo.y;


		// GRAVITY & FLOOR COLLISION :

		// "touching" floor, several cases:
		if (it->touchingFloor()) {

			// resting ball (vertically) stays resting :
			if (it->_velo.y == 0.f)
				it->_pos.y = -1.f + it->_radius;

			
			// too slow - "rest" the ball
			else if (it->_velo.y < BOUNCE_THRESHOLD && it->_velo.y > -BOUNCE_THRESHOLD) {
				it->_pos.y = it->_radius - 1;
				it->_velo.y = 0.f;
				//printf("static\n");
			}

			// not too slow up - apply gravity
			else if (it->_velo.y > 0) {
				/*printf("floor up\n");
				printf("%f\n", it->_velo.y / (gDeltaTime * GRAVITY_POWER));*/
				it->_velo.y -= gDeltaTime * GRAVITY_POWER;
			}

			// going down inside floor -  don't apply gravity, bounce back off floor
			else {
				/*printf("floor down\n");
				printf("%f\n", it->_velo.y / (gDeltaTime * GRAVITY_POWER));*/
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
	float lightPoint[2];
	float distFromLight;
	for (auto it = _balls.begin(); it != _balls.end(); ++it)
	{
		// center, radius and color:
		glUniform2f(_centerUV, it->_pos.x, it->_pos.y);
		glUniform1f(_radiusUV, it->_radius);
		glUniform4f(_fillColorUV, it->_color[0], it->_color[1], it->_color[2], it->_color[3]);

		// lighting, extract the light hit pos on the ball
		distFromLight = std::sqrt(std::pow(it->_pos.x - _lightPos[0], 2) +
								  std::pow(it->_pos.y - _lightPos[1], 2));
		lightPoint[0] = it->_pos.x + 0.33f * (_lightPos[0] - it->_pos.x) * it->_radius / distFromLight;
		lightPoint[0] = it->_pos.y + 0.33f * (_lightPos[1] - it->_pos.y) * it->_radius / distFromLight;
		
		glUniform2f(_lightUV, it->_pos.x + 0.33f * (_lightPos[0] - it->_pos.x) * it->_radius / distFromLight,
			it->_pos.y + 0.33f * (_lightPos[1] - it->_pos.y) * it->_radius / distFromLight);

		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_EDGES_AMOUNT + 2);
		
	}
}


add  reshape function;
