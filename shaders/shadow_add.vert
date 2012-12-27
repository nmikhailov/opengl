#version 330

// In
in vec4 vert;
in vec2 pos;

// Out
out vec4 vertex_light;
out vec4 vertex;
out vec2 uv;

// Uniform
uniform mat4x4 bias;
uniform mat4x4 PVM, LPVM;
uniform mat4x4 M;

void main() {
    gl_Position = PVM * vert;
    vertex_light = LPVM * vert * bias;
    vertex = M * vert;
    uv = pos;
}
