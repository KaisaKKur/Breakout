#version 460 core

in vec2 texture_;
out vec4 color;

uniform sampler2D   scene;
uniform vec2        offsets[9];
uniform int         edges[9];
uniform float       blurs[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main() {
    color = vec4(0.0f);

    vec3 samples[9];

    // 如果使用卷积矩阵，则从纹理偏移中采样
    if(chaos || shake) {
        for(int i = 0; i < 9; i++) {
            samples[i] = vec3(texture(scene, texture_ + offsets[i]));
        }
    }

    // 过程效果
    if(chaos) {           
        for(int i = 0; i < 9; i++) {
            color += vec4(samples[i] * edges[i], 0.0f);
        }
        color.a = 1.0f;
    } else if(confuse) {
        color = vec4(1.0 - texture(scene, texture_).rgb, 1.0);
    } else if(shake) {
        for(int i = 0; i < 9; i++) {
            color += vec4(samples[i] * blurs[i], 0.0f);
        }
        color.a = 1.0f;
    } else {
        color =  texture(scene, texture_);
    }
}
