#version 330

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

    mat4x4 PV_light;

    vec3 att;
};


const int max_lights = 20;
uniform sampler2D shadows[max_lights];

uniform int lightCnt;
uniform Light lights[max_lights];

in vec4 vertex_light[max_lights];

float Epsilon = 0.01;
float Epsilon2 = 0;
float BacklightValue = 0.2;

float myShadow2DProj(sampler2D shadow, vec4 vertex_light, float bias) {
    float depth = texture2DProj(shadow, vertex_light.xyw).z;
    //depth = clamp(depth + Epsilon2, 0, 1);
    return (depth < vertex_light.z / vertex_light.w) ? 0. : 1.;
}

void main(void) {
    if(color_mode == 0) {
        color = cl_color;
    } else if(color_mode == 1){
        color = v_color;
    } else {
        color = texture2D(tex, uv);
    }

   // if(color.a == 0)
   //     discard;

    if (color.rgb == vec3(62, 88, 59) / 255) {
        discard;
    }

    //
    vec4 light = 0;
    for (int i = 0; i < lightCnt; i++) {
        //i = 1;
        Light l = lights[i];

        vec3 vertexToLightSource = l.position - vertex;
        vec3 lightDirection = normalize(vertexToLightSource);

        vec4 diffuseReflection;
        float ccos = dot(lightDirection, -l.direction);

        // on direct light ?
        bool onLight = true;
        float visibility = 1.;

        if (ccos < l.cosAngle) {
            onLight = false;
            visibility = 0;
        } else {
            visibility = myShadow2DProj(shadows[i], vertex_light[i], 0.2);
        }

        float dist = length(vertexToLightSource);
        float attenuation = 1.0 / (l.att.x + l.att.y * dist + l.att.z * dist * dist);

        diffuseReflection = l.diffuse * clamp(dot(normal, lightDirection), 0, 1) * attenuation;
        visibility = clamp(visibility, BacklightValue, 1);

        vec4 cur_val = clamp(diffuseReflection * visibility, BacklightValue, 1);
        //vec4 cur_val = diffuseReflection * visibility;
        //vec4 cur_val = visibility;

        if(i == 0) {
            light = cur_val;
        } else {
            light += cur_val;
            //light += diffuseReflection;
        }
    }

    color *= light;
}
