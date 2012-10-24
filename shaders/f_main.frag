#version 130

in vec4 v_color;
flat in vec3 normals;
in vec2 uv;

uniform vec4 cl_color;
uniform int color_mode;
uniform sampler2D tex;

out vec4 color;

void main(void) {
    if(color_mode == 0) {
        color = cl_color;
    } else if(color_mode == 1){
        color = v_color;
    } else {
        color = texture2D(tex, uv);
    }
}
