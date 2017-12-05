// OpenGL includes
#include <GL/glew.h>		// typedef GLuint;

// Lib includes
#include <ogldev_util.h>				// ReadFile(); #define INVALID_UNIFORM_LOCATION;
#include <ogldev_callbacks.h>			// ICallbacks;
#include <ogldev_glut_backend.h>		// glut backend calls
#include <ogldev_pipeline.h>
#include <ogldev_camera.h>
#include <ogldev_texture.h>


// General includes
#include <cmath>				// std::sin(); std::cos(); std::rand();
#include <ogldev_math_3d.h>				// Vector3f;
#include <stdio.h>

#define DEBUG

enum ROTATE_AXIS {ROTATE_AXIS_X, ROTATE_AXIS_Y, ROTATE_AXIS_Z};

// GLOBALS 
//int gPOLYGON_MODE = GL_LINE;
int gPOLYGON_MODE = GL_FILL;
ROTATE_AXIS gCURR_ROTATE_AXIS = ROTATE_AXIS_Y;

const char * pVSFileName = "shader.vs";
const char * pFSFileName = "shader.fs";

#define TEXTURE_FILENAME "Textures/test.png"
//#define TEXTURE_FILENAME "Textures/checkers.gif"

#define WINDOW_WIDTH	768
#define WINDOW_HEIGHT	768



#define SPHERE_SEGMENTS	10
#define NUM_OF_VERTICES ((4 * SPHERE_SEGMENTS + 1) * (2 * SPHERE_SEGMENTS + 1))
#define NUM_OF_INDICES (3 * 4 * SPHERE_SEGMENTS * (2 + 2 * (2 * SPHERE_SEGMENTS - 2)))

struct Vertex {
	Vector3f _pos;
	Vector2f _tex;
	Vertex(){}
	Vertex(Vector3f pos, Vector2f tex) : _pos(pos), _tex(tex){}
	void print() {
		_pos.Print();
		_tex.Print();
	}
};

struct App : ICallbacks {

	App() : _vbo(0),
			_ibo(0),
			_texture(GL_TEXTURE_2D, TEXTURE_FILENAME){}

	~App() {
		if (_vbo != 0)
			glDeleteBuffers(1, &_vbo);

		if (_ibo != 0)
			glDeleteBuffers(1, &_ibo);
		
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
		case OGLDEV_KEY_k:
			_animate = !_animate;
			break;
		case OGLDEV_KEY_m:
			gPOLYGON_MODE = (gPOLYGON_MODE == GL_LINE) ? GL_FILL : GL_LINE;
			glPolygonMode(GL_FRONT_AND_BACK, gPOLYGON_MODE);
			_render = true;
			break;
		case OGLDEV_KEY_x:
			gCURR_ROTATE_AXIS = ROTATE_AXIS_X;
			break;
		case OGLDEV_KEY_y:
			gCURR_ROTATE_AXIS = ROTATE_AXIS_Y;
			break;
		case OGLDEV_KEY_z:
			gCURR_ROTATE_AXIS = ROTATE_AXIS_Z;
			break;
		default:
			if (_camera.OnKeyboard(OgldevKey))
				//GLUTBeckendPostRedisplay();
				_render = true;
		}
	}

	void PassiveMouseCB(int x, int y) {
		_camera.OnMouse(x, y);
		//GLUTBeckendPostRedisplay();
		_render = true;
	}

	void IdleCB() {
		if (_render |= _animate)
			RenderSceneCB();
	}

	bool Init() {

		// Initialize GLUT Backend
		GLUTBackendInit(0, nullptr, true, false);

		// Create window & init glew
		if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Sphere"))
			return false;

		// Compile shaders
		CompileShaders();

		
		// Init Texture
		if (!_texture.Load())
			return false;
		_texture.Bind(GL_TEXTURE0);	

		// Texture unit's Uniform variables
		glUniform1i(_textureUnitLocation, 0);// Should correspond to the texture target

		// Set the Perspective Projection values
		_projection.FOV = 60.0f;
		_projection.Width = WINDOW_WIDTH;
		_projection.Height = WINDOW_HEIGHT;
		_projection.zNear = 0.1f;
		_projection.zFar = 400.0f;
		
		// Init Camera
		/*_camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, 
						{ 0.0f, 0.0f, -4.0f },
						{ 0.0f, 0.0f, 1.0f },
						{ 0.0f, 1.0f, 0.0f });*/
		_camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT,
						{ 0.0f, 0.0f, 4.0f },
						{ 0.0f, 0.0f, -1.0f },
						{ 0.0f, 1.0f, 0.0f });
		
	
		
		// Create vertex and index buffer
		createVertexBuffer();
		createIndexBuffer();

		
		glPolygonMode(GL_FRONT_AND_BACK, gPOLYGON_MODE);

		// Animate at start
		_animate = true;

		// Render at start
		_render = true;

		
		return true;
	}

	void RenderSceneCB() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_camera.OnRender();

		static Vector3f scale(0.0f, 0.0f, 0.0f);
		if(_animate)
			switch (gCURR_ROTATE_AXIS) {
			case ROTATE_AXIS_X:
				scale.x += 0.5f;
				break;
			case ROTATE_AXIS_Y:
				scale.y += 0.5f;
				break;
			default:
				scale.z += 0.5f;
				break;
			}

		Pipeline P;
		P.Rotate(scale);
		P.SetPerspectiveProj(_projection);
		P.SetCamera(_camera);
		glUniformMatrix4fv(_wvpLocation, 1, GL_TRUE, P.GetWVPTrans());
		

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(Vector3f));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glDrawElements(GL_TRIANGLES, NUM_OF_INDICES, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		GLUTBackendSwapBuffers();

		// Reset '_render'
		_render = _animate;
		
	}

private:

	void createVertexBuffer() {

		const int n = SPHERE_SEGMENTS;
		const float delta = M_PI_2 / n;
		float cosa, sina, cosb, sinb;
		int a, b, i = 0;

		Vertex vertices[NUM_OF_VERTICES];

		for (a = 0; a < 2 * n + 1; a++) {

			sina = std::sin((float)a * delta);
			cosa = std::cos((float)a * delta);

			for (b = 0; b < 4 * n + 1; b++, i++) {
				cosb = std::cos((float)b * delta);
				sinb = std::sin((float)b * delta);

				if (a == 0 || a == 2 * n)
					vertices[i]._pos = Vector3f(0.f, 0.f, cosa);
				else
					vertices[i]._pos = Vector3f(cosb * sina, sinb * sina, cosa);


				vertices[i]._tex = Vector2f((float)b / (4 * n), (float)a / (2 * n));
			}
		}

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	void createIndexBuffer() {

		const int n = SPHERE_SEGMENTS;
		int a, b, i = 0;
		GLuint indices[NUM_OF_INDICES];
		
		for (a = 0; a < 2 * n; a++)
			for (b = 0; b < 4 * n; b++) {
				
				if (a == 0) {
					indices[i] = a * (4 * n + 1) + b;
					indices[i + 1] = (1 + a) * (4 * n + 1) + b;
					indices[i + 2] = (1 + a) * (4 * n + 1) + b + 1;
					i += 3;
				}
				
				else if (a == 2 * n - 1) {
					indices[i] = (1 + a) * (4 * n + 1) + b;
					indices[i + 1] = a * (4 * n + 1) + b + 1;
					indices[i + 2] = a * (4 * n + 1) + b;
					i += 3;
				}

				else {
					indices[i] = a * (4 * n + 1) + b;
					indices[i + 1] = (1 + a) * (4 * n + 1) + b;
					indices[i + 2] = (1 + a) * (4 * n + 1) + b + 1;

					indices[i + 3] = a * (4 * n + 1) + b;
					indices[i + 4] = (1 + a) * (4 * n + 1) + b + 1;
					indices[i + 5] = a * (4 * n + 1) + b + 1;
					i += 6;
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

		_textureUnitLocation = glGetUniformLocation(ShaderProgram, "gTextureUnit");
		assert(_textureUnitLocation != INVALID_UNIFORM_LOCATION);
	}

	PersProjInfo _projection;
	Camera _camera;
	Texture _texture;
	GLuint _vbo, _ibo;
	GLuint _wvpLocation, _textureUnitLocation;
	bool _render;
	bool _animate;

};


int main(int argc, char ** argv) {
	printf("%d  %d\n", NUM_OF_VERTICES, NUM_OF_INDICES);
	App app;
	if (!app.Init())
		return 1;
	app.Run();
}



