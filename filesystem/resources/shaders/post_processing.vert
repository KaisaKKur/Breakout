#version 460 core

layout (location = 0) in vec2 _position;
layout (location = 1) in vec2 _texture;


out vec2 texture_;

uniform bool    chaos;
uniform bool    confuse;
uniform bool    shake;
uniform float   time;

void main()
{
    gl_Position = vec4(_position, 0.0f, 1.0f); 
    if(chaos) {
        float strength = 0.3;
        vec2 pos = vec2(_texture.x + sin(time) * strength, _texture.y + cos(time) * strength);        
        texture_ = pos;
    } else if(confuse) {
        texture_ = vec2(1.0 - _texture.x, 1.0 - _texture.y);
    } else {
        texture_ = _texture;
    }
    
    if (shake) {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}
