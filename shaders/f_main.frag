#version 120

varying vec4 v_color;
uniform bool texturing_enabled;
uniform bool colormap_enabled;
uniform vec4 cl_color;

void main(void) {
    gl_FragColor = v_color;
}
