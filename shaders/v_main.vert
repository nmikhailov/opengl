#version 130

in vec4 vert;
in vec4 color;
in vec2 uv_buf;
in vec3 norm_buf;

out vec4 v_color;
out vec2 uv;
out vec4 light;

uniform mat4x4 M, V, P;
uniform vec4 cl_color;
uniform bool oneColor;


struct Light {
    vec3 diffuse;
    vec3 position;

    float constantAttenuation, linearAttenuation, quadraticAttenuation;
};


void main() {
    gl_Position = P * V * M * vert;
    v_color = color;
    uv = uv_buf;

    // Compute light
    Light l;
    l.diffuse = normalize(vec3(1, 1, 1));
    l.position = vec3(0, 1, 0);
    l.constantAttenuation = 0;
    l.linearAttenuation = 0;
    l.quadraticAttenuation = 0.05;


    // Light 2
    vec3 normal = (M * vec4(norm_buf, 0)).xyz;
    vec3 vertexToLightSource = vec3(l.position - (M * vert).xyz);
    float distance = length(vertexToLightSource);

    vec3 lightDirection = normalize(vertexToLightSource);
    float attenuation = 1.0 / (l.constantAttenuation
                          + l.linearAttenuation * distance
                          + l.quadraticAttenuation * distance * distance);

    //vec3 diffuseReflection = vec3(l.diffuse) * attenuation * max(0.0, dot(normal, lightDirection));
    vec3 diffuseReflection = l.diffuse * clamp(dot(normal, lightDirection), 0.1, 1) * attenuation;

    light = vec4(diffuseReflection, 1.0);
}
