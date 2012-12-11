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

uniform sampler2DShadow shadow;
in vec4 vertex_light;

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

//float CalcShadowFactor(vec4 LightSpacePos)
//{
//    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
//    vec2 UVCoords;
//    UVCoords.x = ProjCoords.x;
//    UVCoords.y = ProjCoords.y;
//    float z = ProjCoords.z;
//    float Depth = shadow2D(shadow, UVCoords).x;
//    if (Depth < (z + 0.00001))
//        return 0.5;
//    else
//        return 1.0;
//}
float Epsilon = 0.00001;
float Epsilon2 = 10;

float lookup(float x, float y) {
    float depth = shadow2DProj(shadow, vertex_light + vec4(x, y, 0, 0) * Epsilon, Epsilon2).x;
    //depth = clamp(depth + Epsilon2, 0, 1);
    return depth != 1.0 ? 0.25 : 1.0;
}

void main(void) {
    if(color_mode == 0) {
        color = cl_color;
    } else if(color_mode == 1){
        color = v_color;
    } else {
        color = texture2D(tex, uv);
    }

    //
    bool inLight = false;
    vec4 light;
    for (int i = 0; i < lightCnt; i++) {
        Light l = lights[i];

        vec3 vertexToLightSource = l.position - vertex;
        vec3 lightDirection = normalize(vertexToLightSource);

        vec4 diffuseReflection;
        float ccos = dot(lightDirection, -l.direction);
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

    //float visibility = texture2D(shadow, gl_FragCoord.xy / 1024).z;
    //val = CalcShadowFactor(vertex_light);

    float sum = 0.0;
    sum += lookup(-0.5, -0.5);
    sum += lookup( 0.5, -0.5);
    sum += lookup(-0.5, 0.5);
    sum += lookup( 0.5, 0.5);
    sum /= 4;

    //color *= sum;
    color *= lookup(0, 0);
    //float x = color;
    //color = val;
    //color = color + 0.0001;

    //if (val != 1.)
        //color = 0.1;
}
