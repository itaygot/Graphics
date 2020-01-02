#version 330

layout(location = 0) in vec2 position;
uniform vec2	ballCenter;
uniform float	ballRadius;
//uniform float	wallPos;
//uniform float	ceilingPos;

////////////////////////
uniform float	worldXRadius;
uniform float	worldYRadius;
////////////////////////


out vec2 worldPos;

void main()
{
	worldPos = ballRadius * position.xy + ballCenter;
	vec2 temp = worldPos / vec2(worldXRadius, worldYRadius);
	gl_Position = vec4(temp.x, temp.y, 0.0, 1.0);
	
}
