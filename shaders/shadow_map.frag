#version 330

layout(location = 0) out float color;

void main(){
    // useless
    color = gl_FragCoord.z;
    //color = vec4(1, 1, 0, 0);
}
