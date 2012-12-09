#version 130

out float color;

void main(){
    color = gl_FragCoord.z;
    //color = vec4(1, 1, 0, 0);
}
