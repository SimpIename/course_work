#version 150

in vec2 vertex;

void main() {
    gl_Position = vec4(vertex, 0.5, 1);
}
