#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 passUV;
out vec3 passNormal;
out vec3 fragPos;

uniform mat3 inverseModel;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	passUV = aUV;
	passNormal = normalize(inverseModel * aNormal);
	fragPos = vec3(model * vec4(aPos, 1.0));

	gl_Position = projection * view * model * vec4(aPos, 1.0);

}