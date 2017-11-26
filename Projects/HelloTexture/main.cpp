// OpenGL includes
#include <GL/glew.h>		// typedef GLuint;

// Lib includes
#include <ogldev_callbacks.h>			// ICallbacks;
#include <ogldev_glut_backend.h>		// glut backend calls
#include <ogldev_texture.h>
#include <ogldev_math_3d.h>				// Vector3f;
#include <ogldev_util.h>				// ReadFile(); #define INVALID_UNIFORM_LOCATION;

// General includes
#include <stdio.h>


struct Vertex{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex){
		m_pos = pos;
		m_tex = tex;
	}
};


// Globals
const char * pVSFileName = "shader.vs";
const char * pFSFileName = "shader.fs";


#define WINDOW_WIDTH	768
#define WINDOW_HEIGHT	768
#define TEXTURE_FILENAME "Textures/test.png"


struct App : ICallbacks {

	/*App() : _texture(GL_TEXTURE_2D, "Textures/test.png"){

	}*/

	//App() {}
	App() : _texture(GL_TEXTURE_2D, TEXTURE_FILENAME){}

	~App() {
		if (_vbo != 0)
			glDeleteBuffers(1, &_vbo);

		/*if (_textureObject != 0)
			glDeleteTextures(1, &_textureObject);*/
	}

	bool Init() {

		// Initialize GLUT Backend
		GLUTBackendInit(0, nullptr, true, false);

		// Create window & init glew
		if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Texture Test"))
			return false;

		// Compile shaders
		CompileShaders();

		// set Shader's texture location value once
		glUniform1i(_textureUnitLocation, 0);		// Should correspond to the target the texture is bound to

		// Init the texture object
		if (!_texture.Load())
			return false;

		// Load texture
		/*_textureObject = SOIL_load_OGL_texture(TEXTURE_FILENAME,
												SOIL_LOAD_AUTO,
												SOIL_CREATE_NEW_ID,
												0);
		if (_textureObject == 0) {
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
			return false;
		}*/

		// Create vertex buffer
		createVertexBuffer();

		return true;
	}

	void RenderSceneCB() {

		// Clear the screen buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind texture object
		_texture.Bind(GL_TEXTURE0);
		
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(Vector3f)));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		GLUTBackendSwapBuffers();
	}

	void Run() {
		GLUTBackendRun(this);
	}

	void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState) {

		if (OgldevKey == OGLDEV_KEY_q)
			GLUTBackendLeaveMainLoop();

	}

private:
	Texture _texture;
	GLuint _vbo;
	GLuint _textureUnitLocation;


	void createVertexBuffer() {
		Vertex vertices[] = { Vertex(Vector3f(-1.0f, -1.0f, 0.99f), Vector2f(0.f, 0.f)),
							Vertex(Vector3f(0.f, 1.f, 0.99f), Vector2f(0.5f, 1.0f)),
							Vertex(Vector3f(1.0f, -1.0f, 0.99f), Vector2f(1.0f, 0.0f)) };

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

		_textureUnitLocation = glGetUniformLocation(ShaderProgram, "gTextureUnit");
		assert(_textureUnitLocation != INVALID_UNIFORM_LOCATION);
	}

	
};


int main(int argc, char ** argv) {
	App app;
	if (!app.Init())
		return 1;
	app.Run();
}

