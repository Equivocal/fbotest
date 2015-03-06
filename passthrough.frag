#version 400

in vec2 texCoordV;

out vec4 fragColor;

uniform sampler2D texUnit;

void main() {
	fragColor = texture(texUnit, texCoordV);
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}