#version 330

#define PI 3.14

in vec2 texCoordV;

uniform sampler2D texUnit;

out vec4 fragColor;

void main(void) {
    float distance = 1.0;

    for (float y = 0.0; y <= 256.0; y+=1.0) {
        float r = 1.0-y/256.0;
        float theta = 2.0*PI*texCoordV.x;

        vec2 coords = vec2((1.0 + r*cos(theta))/2.0, (1.0 + r*sin(theta))/2.0);
        //vec2 coord = vec2(r, theta/(2.0*PI));

        vec4 data = texture(texUnit, coords);

        float dst = r;
        float caster = data.r;
        if (caster > 0.75) {
            distance = min(distance, dst);
        }
    }

    fragColor = vec4(vec3(distance),1.0);
}