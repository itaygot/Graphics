/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <Magick++.h>

class Texture
{
public:
	Texture();

    Texture(GLenum TextureTarget, const std::string& FileName);

	void SetParameters(GLenum TextureTarget, const std::string & Filename);
	
    bool Load();

    void Bind(GLenum TextureUnit);	// Should correspond to the value passed to the shader e.g. 'unit GL_TEXTURE_0 <=> shader sampler's UV is 0'

private:
    std::string m_fileName;
    GLenum m_textureTarget;			// e.g. 'GL_TEXTURE_2D' 'GL_TEXTURE_1D' etc'
    GLuint m_textureObj;			// Handle to the OGL object (similar to _vbo;)
    Magick::Image m_image;			// Magick::Image object; Extracting texture files.
    Magick::Blob m_blob;			// Binary Large Object - holding texture data
};


#endif	/* TEXTURE_H */

