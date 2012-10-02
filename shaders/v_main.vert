#version 120
#extension GL_ARB_explicit_attrib_location : require

layout(location=0) in vec4 vert;
layout(location=1) in vec4 color;
layout(location=2) in vec2 uv_buf;

varying vec4 v_color;
varying vec2 uv;
uniform mat4x4 proj;

uniform vec4 cl_color;
uniform bool oneColor;

void main() {
    gl_Position = proj * vert;
    v_color = color;
    uv = uv_buf;
}
