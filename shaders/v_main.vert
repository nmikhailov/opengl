#version 120
#extension GL_ARB_explicit_attrib_location : require

layout(location=0) in vec4 vert;
layout(location=1) in vec4 color;

varying vec4 v_color;
uniform mat4x4 proj;


void main() {
    gl_Position = proj * vert;
    v_color = color;
}
