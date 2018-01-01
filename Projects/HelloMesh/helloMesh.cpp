// OpenGL includes
#include <GL/glew.h>		// typedef GLuint;

// Lib includes
#include <ogldev_util.h>				// ReadFile(); #define INVALID_UNIFORM_LOCATION;
#include <ogldev_callbacks.h>			// ICallbacks;
#include <ogldev_glut_backend.h>		// glut backend calls
#include <ogldev_pipeline.h>
#include <ogldev_camera.h>
#include <ogldev_mesh.h>

// General includes
#include <ogldev_math_3d.h>				// Vector3f;
#include <stdio.h>

#define DEBUG

// Globals
const char * pVSFileName = "shader.vs";
const char * pFSFileName = "shader.fs";


#define WINDOW_WIDTH	768
#define WINDOW_HEIGHT	768
#define MESH_FILENAME "../../Content/BobLampClean/boblampclean.md5mesh"
//#define MESH_FILENAME "../../Content/lowpolycat/cat.obj"



struct App : ICallbacks {

	App() {}

	~App() {
		
	}

	bool Init() {

		// Initialize GLUT Backend
		GLUTBackendInit(0, nullptr, true, false, true);

		// Create window & init glew
		if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Texture Test"))
			return false;

		// Compile shaders
		CompileShaders();

		// set Shader's texture location value once
		glUniform1i(_textureUnitLocation, 0); // That's the tex' unit the mesh uses.
		
		// Set the Perspective Projection values
		_projection.FOV = 60.0f;
		_projection.Width = WINDOW_WIDTH;
		_projection.Height = WINDOW_HEIGHT;
		_projection.zNear = 1.0f;
		_projection.zFar = 400.0f;

		// Init Camera
		_camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);


		// Load mesh
		if (!_mesh.LoadMesh(MESH_FILENAME))
			return false;

		return true;
	}

	void RenderSceneCB() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_camera.OnRender();

		Pipeline P;
		P.WorldPos(0.f, 0.f, 40.f);
		P.Scale(Vector3f(0.1f, 0.1f, 0.1f));
		P.SetPerspectiveProj(_projection);
		//P.SetCamera({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		P.SetCamera(_camera);
		glUniformMatrix4fv(_wvpLocation, 1, GL_TRUE, P.GetWVPTrans());

		_mesh.Render();

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
	
	Mesh _mesh;
	PersProjInfo _projection;
	Camera _camera;
	GLuint _textureUnitLocation;
	GLuint _wvpLocation;


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

		_textureUnitLocation = glGetUniformLocation(ShaderProgram, "gTextureUnit");
		assert(_textureUnitLocation != INVALID_UNIFORM_LOCATION);
		
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

