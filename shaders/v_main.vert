#version 130

in vec4 vert;
in vec2 uv_buf;
in vec3 norm_buf;

out vec3 vertex;
out vec3 normal;
out vec2 uv;
//out vec4 light;

uniform mat4x4 M, V, P;
uniform mat3x3 M_N;

void main() {
    gl_Position = P * V * M * vert;

    // To pixel shader
    uv = uv_buf;
    vertex = vec4(M * vert).xyz;
    normal = M_N * norm_buf;
    // Precalc
}
