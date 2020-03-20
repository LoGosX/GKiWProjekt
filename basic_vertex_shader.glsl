#version 330 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 vColor;

void main()
{
	vec4 world_position = M * vertexPosition;
	gl_Position = P * V * world_position;

	vColor = vertexColor;
}