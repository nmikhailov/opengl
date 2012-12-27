#version 330

in vec2 uv;
out vec2 UV;

void main() {
    gl_Position = vec4(uv, 0, 1);
    UV = (uv + vec2(1, 1)) / 2.0;
}
