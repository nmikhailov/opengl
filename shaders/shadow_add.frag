#version 330

// In
in vec4 vertex;
in vec4 vertex_light;
in vec2 uv;

// Out
out vec4 color;

// Uniform
uniform sampler2D bufferIn;
uniform sampler2D shadowIn;
uniform vec3 lpos;
uniform vec3 lview;
uniform float langle;

// Const
float Epsilon = 0.01;
float Epsilon2 = 0;
float BacklightValue = 0.2;

float myShadow2DProj(vec4 vertex_light) {
    float depth = texture2DProj(shadowIn, vertex_light.xyw).z;
    return (depth < vertex_light.z / vertex_light.w) ? 0. : 1.;
}

void main(void) {
    // Previous value
    //color = texture2D(shadowIn, uv);
    color = 0;
    // Light
    float ccos = dot(normalize(lpos - vertex.xyz), -lview);

    if (ccos > langle) {
        color += myShadow2DProj(vertex_light);
        color = 1;
    }
}
