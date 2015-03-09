#version 330

in vec2 vertexPosition;
in vec2 texCoord;

#define PI 3.14

out vec2 texCoordV;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main() {
	//texCoordV = texCoord;

	vec4 pos = projection*view*vec4(vertexPosition, 0.0, 1.0);
	float r = length(pos.xy);
	float theta = atan(pos.y, pos.x)/2.0*PI;

	texCoordV = vec2(theta, r);

	gl_Position = projection*view*vec4(vertexPosition, 0.0, 1.0);
}