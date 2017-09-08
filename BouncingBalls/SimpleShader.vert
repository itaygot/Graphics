#version 330

layout(location = 0) in vec4 position;
uniform vec2 center;
uniform float radius;
uniform vec2 light;

void main()
{
    gl_Position.xy = radius * position.xy + center;
}
