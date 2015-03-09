#version 330

#define PI 3.14

in vec2 texCoordV;

uniform sampler2D texUnit;

out vec4 fragColor;

void main(void) {
    float r = 1.0-texCoordV.y;
    float theta = 2.0*PI*texCoordV.x;

    vec2 coords = vec2((1.0 + r*cos(theta))/2.0, (1.0 + r*sin(theta))/2.0);
    fragColor = texture(texUnit, coords);
}