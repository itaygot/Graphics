//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "Model.h"
#include "ShaderIO.h"


#include <cmath>				// std::sin(); std::cos(); std::rand();
#include "glm/geometric.hpp"	// normalize(); length();
#include <algorithm>			// min(); max();


#define VERTEX_SHADER_PATH "SimpleShader.vert"
#define FRAGMENT_SHADER_PATH "SimpleShader.frag"
#define PI 3.1415926f


/************************************************************/
//				 STATIC IMPLEMENTATION DETAILS				//
/************************************************************/
#define CIRCLE_EDGES_AMOUNT 30
#define SCALARS_PER_VERTEX 2
#define DFLT_RADIUS 0.1f

#define LIGHT_SOURCE_X 0.0
#define LIGHT_SOURCE_Y -1.0

const int DELTA = 32;
const float SPEED = 0.007f;

static float GRAVITY_PER_FRAME = 1.0f / (16 * 32);
static float BOUNCINESS = 0.85f;
//static float WALL_COLLISION_DECELERATION = 0.9f;
//static float FLOOR_COLLISION_DECELERATION = 0.85f;
//static float BALL_COLLISION_DECELERATION = 0.8f;

/************************************************************/
//															//
/************************************************************/

/*** Helper functions ***/

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

/***********************************************************/



/***************** Circle Implementation *******************/

//Model::Circle::Circle(float x, float y) {
//	_pos.x = x;
//	_pos.y = y;
//}
//
//inline bool Model::Circle::onFloor() {
//	//return _y - _cur_radius <= -1;
//	return _pos.y - _cur_radius <= -1;
//}
//
//inline bool Model::Circle::onRightWall() {
//	//return _x + _cur_radius >= 1;
//	return _pos.x + _cur_radius >= 1;
//}
//
//inline bool Model::Circle::onLeftWall() {
//	//return _x - _cur_radius <= -1;;
//	return _pos.x - _cur_radius <= -1;
//}
//
//inline void  Model::Circle::wallCollison()
//{
//	// side walls_velo. 
//	if((onLeftWall() && _velo.x < 0) || (onRightWall() && _velo.x > 0))
//		_velo.x *= -WALL_COLLISION_DECELERATION;
//
//	// floor
//	if (onFloor() && _velo.y != 0) {
//		if (_velo.y < 0)
//			_velo.y *= -FLOOR_COLLISION_DECELERATION;
//		else if (_velo.y < GRAVITY_PER_FRAME) {
//			_pos.y = _cur_radius - 1;
//			_velo.y = 0;
//		}
//	}
//}
//
//void Model::Circle::ballCollision(Circle& other)
//{
//	glm::vec2 axis;
//	float dist, axis_speed, other_axis_speed, delta_speed;
//
//
//	if ((_pos.x - _cur_radius <= other._pos.x + other._cur_radius) &&
//		(_pos.x + _cur_radius >= other._pos.x - other._cur_radius) &&
//		(_pos.y - _cur_radius <= other._pos.y + other._cur_radius) &&
//		(_pos.y + _cur_radius >= other._pos.y - other._cur_radius))
//		// filtering far away balls
//	{
//		dist = glm::length(other._pos - _pos);
//		if (dist <= _cur_radius + other._cur_radius && dist > 0.f){		// balls touching
//			axis = glm::normalize((other._pos - _pos));
//			axis_speed = glm::dot(_velo, axis);
//			other_axis_speed = glm::dot(other._velo, axis);
//
//			if ((delta_speed = axis_speed - other_axis_speed) > 0) {	// going at eachother
//				_velo -= axis * delta_speed * BALL_COLLISION_DECELERATION;
//				other._velo += axis * delta_speed * BALL_COLLISION_DECELERATION;
//			}
//		}
//	}
//
//	
//}

/************************************************************/



/*** Model Implementations ***/

Model::Model(){}

Model::~Model()
{
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

void Model::init()
{
	// Create the program
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


	/*** Create the general circle's vertices ***/
		
	float vertices[CIRCLE_EDGES_AMOUNT * SCALARS_PER_VERTEX];
		
	int vertex = 0;
	float angle = 0;
	for (vertex = 0; vertex < CIRCLE_EDGES_AMOUNT; vertex++, angle += 2.f * PI / CIRCLE_EDGES_AMOUNT){
		vertices[SCALARS_PER_VERTEX * vertex] = std::cos(angle);		// x coordinate
		vertices[SCALARS_PER_VERTEX * vertex + 1] = std::sin(angle);	// y coordinate	
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
		


	/** light sources **/
	_lightSource[0] = LIGHT_SOURCE_X;
	_lightSource[1] = LIGHT_SOURCE_Y;

}

void Model::draw()
{
	//GLenum polygonMode = GL_POINT;   // Also try using GL_FILL and GL_POINT
	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	
	// Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// Execute balls actions for this round
	moveBalls();
	handleWallsCollisions();
	handleBallsCollisions();
	drawBalls();
	
	
	// Cleanup, not strictly necessary
}

void Model::resize(int width, int height)
{
    _width	= (float)width;
    _height = (float)height;
    _offsetX = 0;
    _offsetY = 0;
}

//void Model::addCircle(float x, float y)
void Model::addBall(float x, float y)
{
	//Circle circle = Circle(x, y);
	Ball ball = Ball(x, y);

	ball._bounciness = 0.85f;

	// Draw a color
	ball._color = pickAColor();

	// Draw a velocity
	ball._velo.x = rand_float(3) * 0.1f / 2;
	//circle._velo.y = rand_float(3) * 0.1f / 2;
	ball._velo.y = 0.f;
	/*circle._dx  = 0;
	circle._dy = 0;*/

	// Find the default radius, first check the walls then the other balls
	float radius = std::min(DFLT_RADIUS, x + 1);
	radius = std::min(radius, 1 - x);
	radius = std::min(radius, y + 1);
	radius = std::min(radius, 1 - y);
	if (radius <= 0.f) return;
	ball._def_radius = (ball._cur_radius = radius);

	// add to balls list
	_balls.push_back(ball);
}


inline void Model::handleBallsCollisions()
{
	for (auto it1 = _balls.begin(); it1 != _balls.end(); it1++)
	for (auto it2 = it1 + 1; it2 != _balls.end(); it2++)
		it1->ballCollision(*it2);
}

inline void Model::moveBalls()
{
	for (auto it = _balls.begin(); it != _balls.end(); it++)
	{
		// restore ball's default radius
		it->_cur_radius = it->_def_radius;

		it->_pos.x += it->_velo.x;
		it->_pos.y += it->_velo.y;

		// apply gravity
		/*if(!it->onFloor())
			it->_velo.y -= GRAVITY_PER_FRAME;	*/
		make resting efficiency;
		if (it->onFloor()) {					// floor touch
			if(it->_velo.y >= 0.f)					// going up
				if (it->_velo.y < GRAVITY_PER_FRAME) {	// too slow, rest the ball
					it->_pos.y = it->_cur_radius - 1;
					it->_velo.y = 0.f;
				}
				else
					it->_velo.y -= GRAVITY_PER_FRAME;
		}

		else
			it->_velo.y -= GRAVITY_PER_FRAME;




	}

	//bool floor;
	//for (auto it = _circles.begin(); it != _circles.end(); it++)
	//{
	//	// restore ball's default radius
	//	it->_cur_radius = it->_def_radius;
	//	floor = false;

	//	it->_x += it->_dx;
	//	it->_y += it->_dy;

	//	if (it->_y <= it->_cur_radius - 1) {			// hitting floor
	//		floor = true;
	//		it->_y = it->_cur_radius - 1;
	//		//it->_dy *= -0.95f;
	//		it->_dy *= -1;
	//	}

	//	

	//	// apply gravity
	//	//if (!floor)
	//		it->_dy -= GRAVITY_PER_FRAME;
	//	
	//	if(floor)
	//		it->_dy *= 0.9f;
	//}
}

inline void Model::handleWallsCollisions()
{
	for (auto it = _balls.begin(); it != _balls.end(); it++)
	{
		it->wallCollison();
	}
}

inline void Model::drawBalls()
{
	float lightPoint[2];
	float distFromLight;
	for (auto it = _balls.begin(); it != _balls.end(); ++it)
	{
		glUniform2f(_centerUV, it->_pos.x, it->_pos.y);
		glUniform1f(_radiusUV, it->_cur_radius);
		glUniform4f(_fillColorUV, it->_color[0], it->_color[1], it->_color[2], it->_color[3]);

		// Deal with lighting... extract the light point on the ball
		distFromLight = std::sqrt(std::pow(it->_pos.x - _lightSource[0], 2) +
								  std::pow(it->_pos.y - _lightSource[1], 2));
		lightPoint[0] = it->_pos.x + 0.33f * (_lightSource[0] - it->_pos.x) * it->_cur_radius / distFromLight;
		lightPoint[0] = it->_pos.y + 0.33f * (_lightSource[1] - it->_pos.y) * it->_cur_radius / distFromLight;
		
		glUniform2f(_lightUV, it->_pos.x + 0.33f * (_lightSource[0] - it->_pos.x) * it->_cur_radius / distFromLight,
			it->_pos.y + 0.33f * (_lightSource[1] - it->_pos.y) * it->_cur_radius / distFromLight);

		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_EDGES_AMOUNT + 2);
		//glDrawArrays(GL_TRIANGLES, 0, CIRCLE_EDGES_AMOUNT + 2);
		//glDrawArrays(GL_LINES, 0, CIRCLE_EDGES_AMOUNT + 2);
		
	}
}

