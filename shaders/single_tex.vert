#version 330

in vec2 uv_coords;
out vec2 UV;

void main() {
    gl_Position = vec4(uv_coords, 0, 1);
    UV = (uv_coords + vec2(1, 1)) / 2.0;
}
