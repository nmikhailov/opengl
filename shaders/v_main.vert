#version 130

in vec4 vert;
in vec4 color;
in vec2 uv_buf;
in vec4 norm_buf;

out vec4 v_color;
out vec2 uv;

uniform mat4x4 proj;
uniform vec4 cl_color;
uniform bool oneColor;

void main() {
    gl_Position = proj * vert;
    v_color = color;
    uv = uv_buf;
}
