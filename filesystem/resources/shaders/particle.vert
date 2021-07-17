#version 460 core

layout (location = 0) in vec2 _position;
layout (location = 1) in vec2 _texture;

out vec2 texture_;
out vec4 particle_color_;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;

void main() {
	float scale = 10.0f;
	texture_ = _texture;
	particle_color_ = color;
	gl_Position = projection * vec4((_position * scale) + offset, 0.0f, 1.0f);
}
