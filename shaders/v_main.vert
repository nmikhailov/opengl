#version 120
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 position;
uniform mat4x4 proj;

void main() {
    gl_Position = proj * position;
}
