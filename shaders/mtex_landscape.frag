#version 120
#extension GL_ARB_explicit_attrib_location : require

in vec2 uv;
in float height;
uniform sampler2D tex1; // grass
uniform sampler2D tex2; // rock
uniform sampler2D tex3; // ice

uniform vec2 prop;

void main(void) {
    float a = prop.x * height;
    float b = prop.y * height;

    vec4 c1 = texture2D(tex1, uv) * (1 - height);
    vec4 c2 = texture2D(tex2, uv) * height;
    vec4 c3 = texture2D(tex3, uv) * height;
    vec4 cr = (c1 + c2);
    cr = (cr * (1 - height) + c3);
    //vec4 cr = (c1 * 0.8 + c2 + c3 * 0.6);
    float m = max(cr.r, max(cr.g, cr.b));

    gl_FragColor = cr * m;
}
