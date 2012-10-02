#version 120
#extension GL_ARB_explicit_attrib_location : require

varying vec4 v_color;
varying vec2 uv;

uniform vec4 cl_color;
uniform int color_mode;

uniform sampler2D tex;

void main(void) {
    if(color_mode == 0) {
        gl_FragColor = cl_color;
    } else if(color_mode == 1){
        gl_FragColor = v_color;
    } else {
        gl_FragColor = texture2D(tex, uv);
        //gl_FragColor = vec4(1, 1, 1, 1);
    }
}
