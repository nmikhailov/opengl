#version 330

in vec3 texCoord;
out vec4 fragColor;
uniform samplerCube cubemap;

void main (void) {
    fragColor = textureCube(cubemap, texCoord);
}
