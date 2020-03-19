#version 330 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 mvp;

out vec4 vColor;

void main()
{
	gl_Position = mvp * vertexPosition;

	vColor = vertexColor;
}