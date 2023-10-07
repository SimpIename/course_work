#version 150

in vec2 vertex;
uniform mat4 mvp_matrix;

void main() {
   gl_Position = mvp_matrix * vec4(vertex, 0.5, 1);
}
