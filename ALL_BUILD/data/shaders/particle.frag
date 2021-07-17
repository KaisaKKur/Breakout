#version 460 core

in vec2 texture_;
in vec4 particle_color_;

out vec4 fragment_color_;

uniform sampler2D sprite;

void main() {
	fragment_color_ = (texture(sprite, texture_) * particle_color_);
}
