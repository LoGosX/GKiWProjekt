#version 330 core

layout(location = 0) in vec4 vertexPosition;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 vColor;

void main()
{
	vec4 position = M * vertexPosition;
	position.y += sin(position.x + position.z);
	gl_Position = P * V * position;

	vColor = vec4(0, 0, 0, 0);
}