#version 330

uniform vec4 ballColor;
uniform vec2 lightHit;
uniform float ballRadius;

in vec2		worldPos;
out vec4	outColor;


void main()
{
	float p = max(0, 1 - 0.5 * length(lightHit - worldPos) / ballRadius);
	outColor = mix(ballColor, vec4(1.0), pow(p , 4));
}
