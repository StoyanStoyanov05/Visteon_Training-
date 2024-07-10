#version 300 es

out highp vec4 FragColor;
in highp vec2 vTexCoords;
in highp vec3 vNormal;

uniform highp vec3 color;

void main()
{
	
	FragColor = vec4(color, 1.0);


}
