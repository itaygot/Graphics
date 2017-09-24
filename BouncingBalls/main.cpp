#include <GL/glew.h>
#include <GL/freeglut.h>


#include "bouncingBalls.h"
#include <ogldev_glut_backend.h>
#include <iostream>

/** Internal Definitions */

#define	WINDOW_SIZE         (600) // initial size of the window               //
#define WINDOW_WIDTH		600
#define WINDOW_HEIGHT		600
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //



/** main function */
int main(int argc, char** argv)
{
	std::cout << "Starting Bouncing Balls 17:20 9/24..." << std::endl;
	
	// Initialize GLUT 
	GLUTBackendInit(argc, argv, false, false);

	// Create window & init glew
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Bouncing Balls"))
		return 1;
	
	
	// Init the Model object
	BouncingBalls app;
	if (!app.Init())
		return 1;
	app.Run();


	return 0;
}


