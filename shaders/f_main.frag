#version 130

in vec4 v_color;
in vec3 normal;
in vec2 uv;

uniform vec4 cl_color;
uniform int color_mode;
uniform sampler2D tex;

out vec4 color;


struct Light {
    vec3 diffuse;
    vec3 position;
};

void main(void) {
    Light l;
    l.diffuse = normalize(vec3(1, 0.1, 0));
    l.position = normalize(vec3(0, 10, 10));

    if(color_mode == 0) {
        color = cl_color;
    } else if(color_mode == 1){
        color = v_color;
    } else {
        color = texture2D(tex, uv);
    }
    // Light
    float cos = clamp(dot(normal, l.diffuse), 0.5, 1);
    color *= cos;
}
