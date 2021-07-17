#version 460 core

in vec2 texture_;

out vec4 fragment_color_;

uniform sampler2D sprite;
uniform vec3 sprite_color;

void main() {    
    fragment_color_ = texture(sprite, texture_) * vec4(sprite_color, 1.0f);
}
