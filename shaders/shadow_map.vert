#version 130

layout(location = 0) in vec3 vertexPosition_modelspace;

struct Light {
    vec4 diffuse;
    vec3 position;

    vec3 att;
};

Light light;

void main(){
    gl_Position =  depthMVP * vec4(vertexPosition_modelspace,1);
}
