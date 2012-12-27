#version 330

in vec4 v_color;
in vec2 uv;
smooth in vec3 vertex;
smooth in vec3 normal;

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

float myShadow2DProj(sampler2D shadow, vec4 vertex_light) {
    float depth = textureProj(shadow, vertex_light.xyw).z;
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
    if (length(color.rgb - vec3(255, 255, 255) / 255) < 0.8) {
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
        float visibility = 0.;

        if (ccos > l.cosAngle) {
            visibility = myShadow2DProj(shadows[i], vertex_light[i]);
        }

        float dist = length(vertexToLightSource);
        float attenuation = 1.0 / (l.att.x + l.att.y * dist + l.att.z * dist * dist);

        diffuseReflection = l.diffuse * dot(normal, lightDirection) * attenuation;

        vec4 cur_val = clamp(diffuseReflection * visibility, BacklightValue, 1);
        //vec4 cur_val = diffuseReflection * visibility;
        //vec4 cur_val = visibility;

        if(i == 0) {
            light = cur_val;
        } else {
            light *= cur_val;
            //light += diffuseReflection;
        }
    }

    color *= light;
}
