#version 130

layout(location = 0) out float color;

void main(){
    color = gl_FragCoord.z;
}
