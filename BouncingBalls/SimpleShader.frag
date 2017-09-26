#version 330

uniform vec4 fillColor;

out vec4 outColor;
uniform vec2 light;
uniform float radius;
vec2 vLight;


void main()
{
	
	
	vec4 whiteColor = vec4(1.0);

	vLight = vec2((light.x + 1) * 600 / 2, (light.y + 1) * 600 / 2);

	float p = max( 0, 1 - length(vLight - gl_FragCoord.xy) / (radius * 600));
	outColor = mix(fillColor, whiteColor, pow(p , 4));
}
