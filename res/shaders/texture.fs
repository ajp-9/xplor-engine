#version 330

out vec4 FragColor;

in vec2 passUV;

uniform sampler2D diffuseMap;

void main()
{
    FragColor = texture(diffuseMap, passUV);
}