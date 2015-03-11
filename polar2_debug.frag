#version 330

#define PI 3.14

in vec2 texCoordV;

uniform sampler2D texUnit;
uniform vec2 winsize;

out vec4 fragColor;

void main(void) {	
	vec2 rel = texCoordV;
	
	float r = rel.y;
    float theta = 2.0*PI*rel.x;

    //vec2 coords = vec2((1.0+r*sin(theta))/2.0, (1.0+r*cos(theta))/2.0);
	vec2 coords = vec2(theta/(2.0*PI), r);
    //fragColor = texture(texUnit, coords);
	fragColor = texture(texUnit, texCoordV);
}