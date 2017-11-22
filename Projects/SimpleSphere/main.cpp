// OpenGL includes
#include <GL/glew.h>		// typedef GLuint;

// Lib includes
#include <ogldev_util.h>				// ReadFile(); #define INVALID_UNIFORM_LOCATION;
#include <ogldev_callbacks.h>			// ICallbacks;
#include <ogldev_glut_backend.h>		// glut backend calls
#include <ogldev_pipeline.h>
#include <ogldev_camera.h>


// General includes
#include <cmath>				// std::sin(); std::cos(); std::rand();
#include <ogldev_math_3d.h>				// Vector3f;
#include <stdio.h>

#define DEBUG

// Globals
const char * pVSFileName = "shader.vs";
const char * pFSFileName = "shader.fs";


#define WINDOW_WIDTH	768
#define WINDOW_HEIGHT	768

#define SPHERE_SEGMENTS	8
#define NUM_OF_VERTICES (2 + 4 * SPHERE_SEGMENTS * (2 * SPHERE_SEGMENTS - 1))
#define NUM_OF_INDICES (3 * 4 * SPHERE_SEGMENTS * (2 + 2 * (2 * SPHERE_SEGMENTS - 2)))

struct Vertex {
	Vector3f _pos;
	Vertex(){}
	Vertex(Vector3f pos) : _pos(pos){}
};


struct App : ICallbacks {

	App() {}

	~App() {
		
	}



	bool Init() {

		// Initialize GLUT Backend
		GLUTBackendInit(0, nullptr, true, false);

		// Create window & init glew
		if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Sphere"))
			return false;

		// Compile shaders
		CompileShaders();

		
		// Set the Perspective Projection values
		_projection.FOV = 60.0f;
		//_projection.FOV = 90.0f;
		_projection.Width = WINDOW_WIDTH;
		_projection.Height = WINDOW_HEIGHT;
		_projection.zNear = 0.1f;
		_projection.zFar = 400.0f;
		
		// Init Camera
		_camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, 
						{ 0.0f, 0.0f, -4.0f },
						{ 0.0f, 0.0f, 1.0f },
						{ 0.0f, 1.0f, 0.0f });
	
		
		// Create vertex and index buffer
		createVertexBuffer();
		createIndexBuffer();

		// Polygon mode:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		return true;
	}

	void RenderSceneCB() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_camera.OnRender();

		Pipeline P;
		P.SetPerspectiveProj(_projection);
		P.SetCamera(_camera);
		glUniformMatrix4fv(_wvpLocation, 1, GL_TRUE, P.GetWVPTrans());
		

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glDrawElements(GL_TRIANGLES, NUM_OF_INDICES, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);

		GLUTBackendSwapBuffers();
		
	}

	void Run() {
		GLUTBackendRun(this);
	}
	
	void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		default:
			if (_camera.OnKeyboard(OgldevKey))
				GLUTBeckendPostRedisplay();
		}
	}

	void PassiveMouseCB(int x, int y) {
		_camera.OnMouse(x, y);
		GLUTBeckendPostRedisplay();
	}

private:
	
	GLuint _vbo, _ibo;
	PersProjInfo _projection;
	Camera _camera;
	GLuint _wvpLocation;
	//Matrix4f _m;

	void createVertexBuffer() {

		const int n = SPHERE_SEGMENTS;
		const float delta = M_PI_2 / n;
		float cosa, sina;
		int a, b, i = 0;

		Vertex vertices[NUM_OF_VERTICES];		

		for (a = 0; a < 2 * n + 1; a++) {

			sina = std::sin((float)a * delta);
			cosa = std::cos((float)a * delta);

			if (a == 0 || a == 2 * n) {
			vertices[i]._pos.x = 0.f;
			vertices[i]._pos.y = 0.f;
			vertices[i]._pos.z = cosa;
			i++;
			continue;
			}

			for (b = 0; b < 4 * n; b++, i++) {
			vertices[i]._pos.x = std::cos((float)b * delta) * sina;
			vertices[i]._pos.y = std::sin((float)b * delta) * sina;
			vertices[i]._pos.z = cosa;
			}
		}
		
		
		/*Vector4f tmp;
		float x, y, z;
		_m.Print();printf("\n");
		for (i = 0; i < 3; i++) {
			x = vertices[i]._pos.x;
			y = vertices[i]._pos.y;
			z = vertices[i]._pos.z;
			printf("\n");	vertices[i]._pos.Print();
			((tmp = _m * Vector4f(x, y, z, 1)) / tmp.w).Print();

		}*/

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	void createIndexBuffer() {

		const int n = SPHERE_SEGMENTS;
		int a, b, nextb, i = 0;
		GLuint indices[NUM_OF_INDICES];


		for (a = 0; a < 2 * n; a++)
			for (b = 0; b < 4 * n; b++) {

				nextb = (b + 1 < 4 * n) ? b + 1 : 0;

				if (a < n) {
					if (a == 0) {				// Positive-Z pole
						indices[i] = 0;
						indices[i + 1] = b + 1;
						indices[i + 2] = nextb + 1;
						i += 3;
					}
					else {
						indices[i] = 1 + (a - 1) * 4 * n + b;	
						indices[i + 1] = 1 + a * 4 * n + nextb;
						indices[i + 2] = 1 + (a - 1) * 4 * n + nextb;

						indices[i + 3] = 1 + (a - 1) * 4 * n + b;
						indices[i + 4] = 1 + a * 4 * n + b;
						indices[i + 5] = 1 + a * 4 * n + nextb;
						i += 6;
					}
				}
				else {
					if (a == 2 * n - 1) {		// Negative-Z Pole
						indices[i] = 1 + a * 4 * n;				
						indices[i + 1] = 1 + (a - 1) * 4 * n + nextb;
						indices[i + 2] = 1 + (a - 1) * 4 * n + b;
						i += 3;
					}
					else {
						indices[i] = 1 + (a - 1) * 4 * n + b;
						indices[i + 1] = 1 + a * 4 * n + nextb;
						indices[i + 2] = 1 + (a - 1) * 4 * n + nextb;

						indices[i + 3] = 1 + (a - 1) * 4 * n + b;
						indices[i + 4] = 1 + a * 4 * n + b;
						indices[i + 5] = 1 + a * 4 * n + nextb;
						i += 6;
					}
				}
			}

		

		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	}
	
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
	{
		GLuint ShaderObj = glCreateShader(ShaderType);

		if (ShaderObj == 0) {
			fprintf(stderr, "Error creating shader type %d\n", ShaderType);
			exit(1);
		}

		const GLchar* p[1];
		p[0] = pShaderText;
		GLint Lengths[1];
		Lengths[0] = strlen(pShaderText);
		glShaderSource(ShaderObj, 1, p, Lengths);
		glCompileShader(ShaderObj);
		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
			exit(1);
		}

		glAttachShader(ShaderProgram, ShaderObj);
	}

	void CompileShaders()
	{
		GLuint ShaderProgram = glCreateProgram();

		if (ShaderProgram == 0) {
			fprintf(stderr, "Error creating shader program\n");
			exit(1);
		}

		string vs, fs;

		if (!ReadFile(pVSFileName, vs)) {
			exit(1);
		};

		if (!ReadFile(pFSFileName, fs)) {
			exit(1);
		};

		AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
		AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

		GLint Success = 0;
		GLchar ErrorLog[1024] = { 0 };

		glLinkProgram(ShaderProgram);
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
		if (Success == 0) {
			glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
			exit(1);
		}

		glValidateProgram(ShaderProgram);
		glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
		if (!Success) {
			glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
			exit(1);
		}

		glUseProgram(ShaderProgram);

		_wvpLocation = glGetUniformLocation(ShaderProgram, "gWVP");
		assert(_wvpLocation != INVALID_UNIFORM_LOCATION);
	}

};


int main(int argc, char ** argv) {
	App app;
	if (!app.Init())
		return 1;
	app.Run();
}



