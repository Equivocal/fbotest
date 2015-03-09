#version 330

#define PI 3.14

//inputs from vertex shader
in vec2 texCoordV;

uniform sampler2D texUnit;

out vec4 fragColor;

float sample(vec2 coord, float r) {
    return step(r, texture(texUnit, coord).r);
}

void main(void) {
    vec2 norm = texCoordV*2.0 - vec2(1.0, 1.0);
    norm.y = -norm.y;
    float theta = atan(norm.y, norm.x);
    float r = length(norm); 
    float coord = (theta) / (PI*2.0);

    vec2 tc = vec2(coord, 0.0);

    float center = sample(tc, r);

    float blur = (1./512.0)  * smoothstep(0., 1., r); 
    float sum = 0.0;

    sum += sample(vec2(tc.x - 4.0*blur, tc.y), r) * 0.05;
    sum += sample(vec2(tc.x - 3.0*blur, tc.y), r) * 0.09;
    sum += sample(vec2(tc.x - 2.0*blur, tc.y), r) * 0.12;
    sum += sample(vec2(tc.x - 1.0*blur, tc.y), r) * 0.15;

    sum += center * 0.16;

    sum += sample(vec2(tc.x + 1.0*blur, tc.y), r) * 0.15;
    sum += sample(vec2(tc.x + 2.0*blur, tc.y), r) * 0.12;
    sum += sample(vec2(tc.x + 3.0*blur, tc.y), r) * 0.09;
    sum += sample(vec2(tc.x + 4.0*blur, tc.y), r) * 0.05;

    fragColor = vec4(1.0, 0.0, 0.0, 1.0) * vec4(sum * smoothstep(1.0, 0.0, r));
}