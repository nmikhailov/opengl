#version 130

in vec4 vert;
in vec4 color;
in vec2 uv_buf;
in vec3 norm_buf;

out vec4 v_color;
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
int max_lights = 50;

uniform int lightCnt;
uniform Light lights[max_lights];

void main() {
    gl_Position = P * V * M * vert;
    v_color = color;
    uv = uv_buf;

    // Light v2
    vec3 normal = M_N * norm_buf;
    for (int i = 0; i < lightCnt; i++) {
        Light l = lights[i];
        vec3 vertexToLightSource = vec3(l.position - vec4(M * vert).xyz);
        vec3 lightDirection = normalize(vertexToLightSource);

        float dist = length(vertexToLightSource);
        float attenuation = 1.0 / (l.att.x + l.att.y * dist + l.att.z * dist * dist);

        vec3 diffuseReflection = l.diffuse * clamp(dot(normal, lightDirection), 0.1, 1) * attenuation;
        if(i == 0) {
            light = vec4(diffuseReflection, 1.0);
        } else {
            light += vec4(diffuseReflection, 1.0);
        }
    }
}
