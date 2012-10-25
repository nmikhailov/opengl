#version 130

in vec4 vert;
in vec4 color;
in vec2 uv_buf;
in vec3 norm_buf;

out vec4 v_color;
flat out vec3 normal;
out vec2 uv;

uniform mat4x4 M, V, P;
uniform vec4 cl_color;
uniform bool oneColor;

void main() {
    gl_Position = P * V * M * vert;
    v_color = color;
    uv = uv_buf;

    vec4 tmp = (M * vec4(norm_buf, 0));
    normal = tmp.xyz;
}
