#version 410 core
layout (location = 0) in vec2 a_position;

out vec2 surfacePosition;
uniform vec2 screenRatio;
uniform vec2 resolution;

void main() {
    surfacePosition = a_position * screenRatio;
    gl_Position = vec4(a_position, 0, 1);
}