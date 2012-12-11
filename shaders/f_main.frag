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
float Epsilon2 = 0.001;
float BacklightValue = 0.2;

vec2 randomNumbers[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

float random(vec3 seed, int i){
    vec4 seed4 = vec4(seed,i);
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float myShadow2DProj(sampler2D shadow, vec4 vertex_light, vec2 v) {
    float depth = texture2DProj(shadow, vertex_light.xyw + vec3(v, 0) * Epsilon).z;
    //depth = clamp(depth + Epsilon2, 0, 1);
    return (depth < (vertex_light.z - Epsilon2) / vertex_light.w ) ? 0. : 1.;
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
    vec4 light;
    for (int i = 0; i < lightCnt; i++) {
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
            for (int j = 0; j < 4; j++){
                // int index = j;
                // int index = int(16.0 * random(gl_FragCoord.xyy, j)) % 16;
                int index = int(16.0 * random(floor(vertex.xyz * 1000.0), j)) % 16;

                visibility -= 0.2 * (1.0 - myShadow2DProj(shadows[i], vertex_light[i], randomNumbers[j]));
            }
        }

        float dist = length(vertexToLightSource);
        float attenuation = 1.0 / (l.att.x + l.att.y * dist + l.att.z * dist * dist);

        diffuseReflection = l.diffuse * clamp(dot(normal, lightDirection), 0.1, 0.9) * attenuation;

        if(i == 0) {
            light = diffuseReflection * visibility;
        } else {
            light += diffuseReflection;
        }
    }

    color *= light;
}
