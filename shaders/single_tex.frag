#version 330

in vec2 UV;
out vec4 color;

uniform sampler2D texture;

const vec2 sz =  vec2(1. / 1091, 1. / 366);
void main(){

    color = texture(texture, UV);
//    color = 2 * texture(texture, UV);
//    color += texture(texture, UV + vec2(sz.x, 0));
//    color += texture(texture, UV + vec2(-sz.x, 0));
//    color += texture(texture, UV + vec2(0, sz.y));
//    color += texture(texture, UV + vec2(0, -sz.y));
//    color /= (4 + 2);
}
