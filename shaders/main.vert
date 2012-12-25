#version 330

in vec4 vert;
in vec2 uv_buf;
in vec3 norm_buf;

smooth out vec3 vertex;
smooth out vec3 normal;
out vec2 uv;
/// Lights
struct Light {
    vec4 diffuse;
    vec3 position;
    vec3 direction;
    float cosAngle;

//    sampler2D shadow;
    mat4x4 PV_light;

    vec3 att;
};



const int max_lights = 20;
uniform sampler2D shadows[max_lights];

uniform int lightCnt;
uniform Light lights[max_lights];
out vec4 vertex_light[max_lights];
// Lights

uniform mat4x4 bias;

uniform mat4x4 M, V, P;
uniform mat3x3 M_N;

void main() {
    gl_Position = P * V * M * vert;

//    for (int i = 0; i < max_lights; i++) {
//        vertex_light[i] = lights[i].PV_light * M * vert * bias;
//    }
    vertex_light[0] = lights[0].PV_light * M * vert * bias;
    //vertex_light[1] = lights[1].PV_light * M * vert * bias;

    // To pixel shader
    uv = uv_buf;
    vertex = vec4(M * vert).xyz;
    normal = M_N * norm_buf;
}
