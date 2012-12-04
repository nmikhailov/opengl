#version 130

in vec4 v_color;
in vec2 uv;
in vec3 vertex;
in vec3 normal;

uniform vec4 cl_color;
uniform int color_mode;
uniform sampler2D tex;
uniform mat4x4 M, V, P;
uniform mat3x3 M_N;

out vec4 color;

struct Light {
    vec4 diffuse;
    vec3 position;
    vec3 direction;
    float cosAngle;

    vec3 att;
};
const int max_lights = 20;

uniform int lightCnt;
uniform Light lights[max_lights];


void main(void) {
    if(color_mode == 0) {
        color = cl_color;
    } else if(color_mode == 1){
        color = v_color;
    } else {
        color = texture2D(tex, uv);
    }

    //
    vec4 light;
    for (int i = 0; i < lightCnt; i++) {
        Light l = lights[i];
        vec3 vertexToLightSource = l.position - vertex;
        vec3 lightDirection = normalize(vertexToLightSource);

        vec4 diffuseReflection;
        float ccos = dot(lightDirection, normalize(-l.direction));
        if (ccos < l.cosAngle) {
            diffuseReflection = 0.1;
        } else {
            float dist = length(vertexToLightSource);
            float attenuation = 1.0 / (l.att.x + l.att.y * dist + l.att.z * dist * dist);

            diffuseReflection = l.diffuse * clamp(dot(normal, lightDirection), 0.1, 0.9) * attenuation;
        }
        if(i == 0) {
            light = diffuseReflection;
        } else {
            light += diffuseReflection;
        }
    }
    color *= light;
}
