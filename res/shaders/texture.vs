#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 passUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    passUV = aUV;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}