#version 330

uniform sampler2D gTextureUnit;

in vec2 TexCoord0;
out vec4 FragColor;



void main()
{
	FragColor = texture2D(gTextureUnit, TexCoord0.xy);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	
	
}

