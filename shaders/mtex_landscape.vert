#version 120
#extension GL_ARB_explicit_attrib_location : require

layout(location=0) in vec4 vert;
layout(location=1) in vec2 uv_coords;

out vec2 uv;
out float height;
uniform mat4x4 proj;

void main() {
    uv = uv_coords;
    height = vert.y + 0.5;

    gl_Position = proj * vert;
}
