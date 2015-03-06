#version 400

in vec2 VertexPosition;
in vec2 texCoord;

out vec2 texCoordV;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	texCoordV = texCoord;
	gl_Position = projection*vec4( VertexPosition, 0.0, 1.0 );
}