#include "ogldev_glew_backend.h"
#include <GL/glew.h>

void GLEWBackendViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}
