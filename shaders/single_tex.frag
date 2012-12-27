#version 330

in vec2 UV;
out vec4 color;

uniform sampler2D texture;

const vec2 sz =  vec2(1. / 1091, 1. / 366);
void main(){

    color = texture(texture, UV);
}
