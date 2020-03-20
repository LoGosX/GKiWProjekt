#version 330

//Zmienne jednorodne
uniform mat4 P = mat4(1.0);
uniform mat4 V = mat4(1.0);
uniform mat4 M = mat4(1.0);

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu


void main(void) {
    gl_Position=P*V*M*vertex;
}
