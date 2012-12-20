#version 330

in vec2 UV;
out vec4 color;

uniform sampler2D tex;

void main(){
    color = texture2D(tex, UV);
    //color = vec4(1, 1, 0, 0.5);
}
