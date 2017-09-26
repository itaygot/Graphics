#version 330

uniform vec4 fillColor;
uniform vec2 light;
uniform float radius;

in vec2 worldPos;
out vec4 outColor;


void main()
{
	
	float p = max(0, 1 - 0.5 * length(light - worldPos) / radius);
	outColor = mix(fillColor, vec4(1.0), pow(p , 4));
}
