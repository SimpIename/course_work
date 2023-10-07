#version 150

//in vec4 color1;
in float radius;
uniform vec4 color;
uniform int point_style;
uniform vec4 view_rect;

out vec4 fragment_color;

float SDF_circle(vec2 p, float radius)
{
    return length(p) - radius;
}
float SDF_fake_triangle(vec2 p, vec2 p0, vec2 p1, vec2 p2)
{
    vec2 e0 = p1 - p0;
    vec2 e1 = p2 - p1;
    vec2 e2 = p0 - p2;

    vec2 v0 = p - p0;
    vec2 v1 = p - p1;
    vec2 v2 = p - p2;

    vec2 o0 = normalize(vec2(e0.y, -e0.x));
    vec2 o1 = normalize(vec2(e1.y, -e1.x));
    vec2 o2 = normalize(vec2(e2.y, -e2.x));

    return max(max(dot(o0,v0), dot(o1,v1)), dot(o2,v2));
}
// Union (A or B)
float csg_union(float d1, float d2)
{ return min(d1,d2); }

// Difference (A not B)
float csg_difference(float d1, float d2)
{ return max(d1,-d2); }

// Intersection (A and B)
float csg_intersection(float d1, float d2)
{  return max(d1,d2); }

// Exclusion (A xor B)
float csg_exclusion(float d1, float d2)
{ return min(max(d1,-d2), max(-d1,d2)); }

void main() {
//   if(length(gl_PointCoord-vec2(0.5)) > 0.5)
//      discard;
//   vec2 pt = gl_PointCoord - vec2(0.5);
//   if(pt.x*pt.x+pt.y*pt.y > 0.25)
//       discard;
//    fragment_color = vec4(1.0,1.0,1.0,1.0);

    if (gl_FragCoord.x < view_rect.x ||
            gl_FragCoord.x > view_rect.z ||
            gl_FragCoord.y < view_rect.y ||
            gl_FragCoord.y > view_rect.w) {
        fragment_color = vec4(0.0);
    } else {
        vec2 p = gl_PointCoord.xy*radius*4 - radius*2;
        float a = 1.0;
    //    float d = length(p) - radius;
        float d = SDF_circle(p, radius);
    //    float d = SDF_fake_triangle(p, vec2(-10, -10), vec2(10, -10), vec2(0, 10));
        if(point_style == 0) {
            if(d > 0.0) a = exp(-d*d*2);
            fragment_color = vec4(color.rgb/2.0, a);
        } else if(point_style == 1) {
            if(abs(d) > 0.0) a = exp(-d*d*2);
            if(d < 0.0) {
                fragment_color = vec4(mix(vec3(1.0), color.rgb/2.0, a), 1.0);
            } else {
                fragment_color = vec4(color.rgb/2.0, a);
            }
        } else {
            if(abs(d) > 0.0) a = exp(-d*d*2);
            if(d < 0.0) {
                fragment_color = vec4(mix(color.rgb, color.rgb/2.0, a), 1.0);
            } else {
                fragment_color = vec4(color.rgb/2.0, a);
            }
        }
    }

//    fragment_color = color1;

//   float a = smoothstep(0.47, 0.5, distance(gl_PointCoord,vec2(0.5)));
//////   fragment_color.rgb = mix(vec3(0.0), vec3(1.0), a);
//   fragment_color.a = mix(fragment_color.a, 0.0, a);
}
