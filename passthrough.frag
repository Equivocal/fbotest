#version 330

in vec2 texCoordV;

#define PI 3.14

out vec4 fragColor;

uniform sampler2D texUnit;

void main() {
	//fragColor = texture(texUnit, texCoordV);
	fragColor = vec4(1.0, 1.0, 1.0, 1.0);

	vec2 norm = texCoordV*2.0 - vec2(1.0,1.0);
	float r = length(norm);
	float theta = atan(norm.y, norm.x)/(2.0*PI);
	if (theta < 0.0) {
        theta = theta + 1.0;
    }


	//fragColor = vec4(theta, theta, 0.0, 1.0);
}