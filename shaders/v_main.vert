#version 130

in vec4 vert;
in vec2 uv_buf;
in vec3 norm_buf;

out vec2 uv;
out vec4 light;

uniform mat4x4 M, V, P;
uniform mat3x3 M_N;
uniform vec4 cl_color;
uniform bool oneColor;

struct Light {
    vec4 diffuse;
    vec3 position;

    vec3 att;
};
const int max_lights = 50;

//uniform int lightCnt;
//uniform Light lights[max_lights];
int lightCnt = 2;
Light lights[2];

void init() {
    lights[0].diffuse = normalize(vec4(1, 1, 1, 1));
    lights[0].position = vec3(-4, 0.1, -1);
    lights[0].att = vec3(0, 0.5, 0);

    lights[1].diffuse = normalize(vec4(1, 1, 1, 1));
    lights[1].position = vec3(-15, 1, 10);
    lights[1].att = vec3(0, 0.5, 0);
}

void main() {
    init();

    gl_Position = P * V * M * vert;
    uv = uv_buf;

    // Light v2
    vec3 normal = M_N * norm_buf;
    for (int i = 0; i < lightCnt; i++) {
        Light l = lights[i];
        vec3 vertexToLightSource = vec3(l.position - vec4(M * vert).xyz);
        vec3 lightDirection = normalize(vertexToLightSource);

        float dist = length(vertexToLightSource);
        float attenuation = 1.0 / (l.att.x + l.att.y * dist + l.att.z * dist * dist);

        vec4 diffuseReflection = l.diffuse * clamp(dot(normal, lightDirection), 0.1, 1) * attenuation;
        if(i == 0) {
            light = diffuseReflection;
        } else {
            light += diffuseReflection;
        }
    }
}
