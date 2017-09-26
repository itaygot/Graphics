#include "bouncingBalls.h"
#include <iostream>



/** main function */
int main(int argc, char** argv)
{
	std::cout << "Starting Bouncing Balls 17:20 9/24..." << std::endl;
	
	
	// Init the Model object
	BouncingBalls & app = BouncingBalls::getInstance();
	if (!app.Init(argc, argv))
		return 1;
	app.Run();


	return 0;
}


