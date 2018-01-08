# Graphics

Graphics projects, using the OpenGL library, for learning purposes.

Source files:
	Projects' source files are under: '<GraphicsRepo>/Projects/'.
	Header files of the 'Common' lib prroject are in: '<GraphicsRepo>/Include/'.



Windows Users:
	Run and build projects through Visual Studio. 
	Solution file at: '<GraphicsRepo>/WindowsSolution'.

Linux Users:
	Build Project: 
		Through corresponding 'makefile', at project folder:
		- 'make clean'
		- 'make build'

	Run Project: 
		Each project executable is under: '<GraphicsRepo>/<ProjectFolder>/dist/'
		*Note*: Because each app depends on its shaders files, run each project
		from its project folder.


Prerequisites:
	- OpenGL version 3.3 and above, Glew, Freeglut.

Using a backend library project ('Common'), written by Etai Meiri, from his OpenGL tutorial website 'http://ogldev.atspace.co.uk/',
some of its files I have edited for my use.

Please contact me at itaygot@gmail.com
