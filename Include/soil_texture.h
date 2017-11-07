
#ifndef TEXTURE_H
#define	TEXTURE_H

#include <GL/glew.h>


#include <string>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);

	bool Load();

	void Bind(GLenum TextureUnit);	// Should correspond to the value passed to the shader e.g. 'unit GL_TEXTURE_0 <=> shader sampler's UV is 0'

private:
	std::string m_fileName;
	GLenum m_textureTarget;			// e.g. 'GL_TEXTURE_2D' 'GL_TEXTURE_1D' etc'
	GLuint m_textureObj;			// Handle to the OGL object (similar to '_vbo;)
};


#endif	/* TEXTURE_H */

