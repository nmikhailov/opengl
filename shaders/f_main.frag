#version 120

varying vec4 v_color;

uniform vec4 cl_color;
uniform bool oneColor;

void main(void) {
    if(oneColor) {
        gl_FragColor = cl_color;
    } else {
        gl_FragColor = v_color;
    }
}
