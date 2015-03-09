#version 330

in vec2 texCoordV;

#define PI 3.14

out vec4 fragColor;

uniform sampler2D texUnit;

void main() {
	fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}