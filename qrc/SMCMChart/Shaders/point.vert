#version 150

in vec2 vertex;
uniform mat4 mvp_matrix;
uniform float point_size;
out float radius;

void main() {
   gl_Position = mvp_matrix * vec4(vertex, 0.5, 1);
   gl_PointSize = point_size*2;
   radius = point_size/2.0;
}
