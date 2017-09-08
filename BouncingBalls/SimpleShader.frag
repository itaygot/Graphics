#version 330

uniform vec4 fillColor;

out vec4 outColor;
uniform vec2 light;
uniform vec2 center;
uniform float radius;
vec2 vLight;


void main()
{
	int cellSize = 10;		// size of a chckboard cell
	vec2 pos = mod(gl_FragCoord.xy,vec2(cellSize * 2));
	
	vec4 whiteColor = vec4(1.0);

	vLight = vec2((light.x + 1) * 600 / 2, (light.y + 1) * 600 / 2);

	float p = min( 1, 0.5 * length(vLight - gl_FragCoord.xy) / (radius * 300));
	outColor = mix(fillColor, whiteColor, pow(1 - p, 4));
}
