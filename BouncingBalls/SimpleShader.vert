#version 330

layout(location = 0) in vec2 position;
uniform vec2 center;
uniform float radius;
uniform vec2 light;

out vec2 worldPos;

void main()
{
    gl_Position.xy = radius * position.xy + center;
	worldPos = gl_Position.xy;
}
