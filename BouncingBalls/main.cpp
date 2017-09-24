//
//  ex0.cpp
//  cg-projects
//
//  Code template by HUJI Computer Graphics course staff, 2013.
//

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

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

#define KEY_ANIMATE         ('k') // Key used to start animation              //
#define KEY_ESC            (27) // Key used to terminate the program - ESC  //
#define KEY_QUIT            ('q') // Key used to terminate the program        //
#define KEY_RESET           ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD          ('l') // Key used to reload the shaders 	      //
#define KEY_WIREFRAME       ('w') // Key used to toggle wireframe rendering   //

/*** Animation Times ***/
#define FRAME_RATE_MILIS 25



/** display callback */
void display(void);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** timer callback */
void timer(int value) ;



// Globals
BouncingBalls g_model;
bool g_animate = true;

/** main function */
int main(int argc, char** argv)
{
	std::cout << "Starting Bouncing Balls 02:00 9/20..." << std::endl;
	
	// Initialize GLUT 
	GLUTBackendInit(argc, argv, false, false);

	// Create window & init glew
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Bouncing Balls"))
		return 1;
	


	// Initialize GLUT
    /*glutInit(&argc, argv) ;
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("Bouncing Balls");*/
	
	// Initialize GLEW
    /*glewExperimental = GL_TRUE;
    int glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(1);
    }*/
	

	// Set callback functions:
    /*glutDisplayFunc(display) ;
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);*/   // uint millis int value
	
	// Init the Model object
	BouncingBalls app;
	if (!app.Init())
		return 1;
	/*g_model.Init();*/

	// Set clear color to black:
	/*glClearColor(0.0, 0.0, 0.0, 0.0);*/
		
	// Start events/drawing loop
	/*glutMainLoop();*/
	app.Run();


	
	return 0;
}

void display(void)
{
	// Let the model to draw itself...
	g_model.RenderSceneCB();	
}


/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);
	
    switch(lower_key)
    {
        case KEY_RESET:
            // reset to initial view of the object
            // For use in a future exercise
            break;
        case KEY_RELOAD:
            // Reload the shading programs of the object
            // For use in a future exercise
            break;
        case KEY_WIREFRAME:
            // Toggle wireframe mode
            // For use in a future exercise
            break;
        case KEY_ANIMATE:
			g_animate = !g_animate;
            break;
        case KEY_QUIT:
        case KEY_ESC:
            // Terminate the program:
            //exit(RC_OK);
			glutLeaveMainLoop();
            break;
        default:
            std::cerr << "Key " << lower_key << " undefined\n";
            break;
    }
    
    return;
}


/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
		g_model.addBall((float)x * 2 / WINDOW_SIZE - 1, 1 - (float)y * 2 / WINDOW_SIZE);
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        
    }
    
    return;
}


void timer(int frame) {

	if (g_animate)
		glutPostRedisplay();

	glutTimerFunc(FRAME_RATE_MILIS, timer, frame);
    
}
