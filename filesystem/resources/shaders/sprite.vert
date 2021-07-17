#version 460 core

layout (location = 0) in vec2 _position; 
layout (location = 1) in vec2 _texture;

out vec2 texture_;

uniform mat4 model;
uniform mat4 projection;

void main() {
    texture_ = _texture;
    gl_Position = projection * model * vec4(_position, 0.0f, 1.0f);
}
