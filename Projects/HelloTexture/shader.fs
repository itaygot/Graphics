#version 330

in vec2 TexCoord0;

out vec4 FragColor;


uniform sampler2D gTextureUnit;

void main()
{
	FragColor = texture2D(gTextureUnit, TexCoord0.xy);
	//FragColor = vec4(TexCoord0.x, 0.0, 0.0, 1.0);
}