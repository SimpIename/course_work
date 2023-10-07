#version 150

//#define ADJACENCY

#ifdef ADJACENCY
layout (lines_adjacency) in;
#else
layout (lines) in;
#endif

layout (triangle_strip, max_vertices = 4) out;

uniform float line_width;
uniform vec2 pixels;

out vec2 v_p, size;
out float line_length;

vec2 toScreenSpace( vec4 vertex )
{
    return vec2( vertex.xy / vertex.w ) * pixels;
}

void main()
{
#ifdef ADJACENCY
    vec2 p0 = toScreenSpace( gl_in[0].gl_Position );
    vec2 p1 = toScreenSpace( gl_in[1].gl_Position );
    vec2 p2 = toScreenSpace( gl_in[2].gl_Position );
    vec2 p3 = toScreenSpace( gl_in[3].gl_Position );

    vec2 vertor = normalize(p2 - p1);
    vec2 normal = vec2(-vertor.y, vertor.x);

    vec2 vertor1 = p1 - p0;
    vec2 vertor2 = p3 - p2;
    float vertor1_length = length(vertor1);
    float vertor2_length = length(vertor2);

    vec2 miter1;
    if (vertor1_length > 0.01) {
         miter1 = normal + vec2(-vertor1.y, vertor1.x) / vertor1_length;
         float length_1 = line_width / dot(miter1, normal);
         miter1 *= length_1/pixels;
    } else {
         miter1 = line_width*normal/pixels;
    }

    vec2 miter2;
    if (vertor2_length > 0.01) {
         miter2 = normal + vec2(-vertor2.y, vertor2.x) / vertor2_length;
         float length_2 = line_width / dot(miter2, normal);
         miter2 *= length_2/pixels;
    } else {
         miter2 = line_width*normal/pixels;
    }

    gl_Position = gl_in[1].gl_Position + vec4(miter1, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position - vec4(miter1, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position + vec4(miter2, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position - vec4(miter2, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
#else
    vec2 p0 = toScreenSpace( gl_in[0].gl_Position );
    vec2 p1 = toScreenSpace( gl_in[1].gl_Position );
    vec2 vertor = p1 - p0;
    float vertor_length = length(vertor);
    if (vertor_length > 0.01) {
        vec2 tangent = vertor / vertor_length;
        vec2 normal = vec2(-vertor.y, vertor.x) / vertor_length;

        float antialias = 5;
        vec4 elongation = vec4((line_width+antialias)*tangent/pixels, 0.0, 0.0);
        vec4 widening = vec4((line_width+antialias)*normal/pixels, 0.0, 0.0);

        gl_Position = gl_in[0].gl_Position + widening - elongation;
        v_p = vec2(-(line_width+antialias), line_width+antialias/2);
        line_length = vertor_length;
        EmitVertex();

        gl_Position = gl_in[0].gl_Position - widening - elongation;
        v_p = vec2(-(line_width+antialias), -(line_width+antialias/2));
        line_length = vertor_length;
        EmitVertex();

        gl_Position = gl_in[1].gl_Position + widening + elongation;
        v_p = vec2(vertor_length + (line_width+antialias), line_width+antialias/2);
        line_length = vertor_length;
        EmitVertex();

        gl_Position = gl_in[1].gl_Position - widening + elongation;
        v_p = vec2(vertor_length + (line_width+antialias), -(line_width+antialias/2));
        line_length = vertor_length;
        EmitVertex();
        EndPrimitive();
    }
#endif
}
