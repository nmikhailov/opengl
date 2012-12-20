#version 330

in vec4 vert;
uniform mat4x4 M, V, P;

void main() {
    gl_Position = P * V * M * vert;
}
