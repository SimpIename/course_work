#version 150

out vec4 fragment_color;
uniform vec4 grid_rect;
uniform vec4 view_rect;

void main()
{
    vec2 scaled_uv = (gl_FragCoord.xy-view_rect.xy) / view_rect.zw * grid_rect.zw + grid_rect.xy;
    vec2 distance_axes_uv = abs(scaled_uv) / fwidth(scaled_uv);
    vec2 distance_major_uv = abs(fract(scaled_uv+0.5)-0.5) / fwidth(scaled_uv);
    vec2 distance_minor_uv;
    distance_minor_uv = min(min(min(abs(fract(scaled_uv)-0.301029996),
                              abs(fract(scaled_uv)-0.477121255)),
                              min(abs(fract(scaled_uv)-0.602059991),
                              abs(fract(scaled_uv)-0.698970004))),
                              min(min(abs(fract(scaled_uv)-0.77815125),
                              abs(fract(scaled_uv)-0.84509804)),
                              min(abs(fract(scaled_uv)-0.903089987),
                              abs(fract(scaled_uv)-0.954242509)))) / fwidth(scaled_uv);

    float dash_length = 8;
    vec2 distance_dash_uv = abs(fract(gl_FragCoord.xy/dash_length)-0.5) * dash_length-1;

    float d_major = min(distance_major_uv.x, distance_major_uv.y);
    float a_major = 1.0;
    if(d_major > 0) {

        a_major = d_major;
        a_major = exp(-a_major*a_major*2);
    }

    float d_minor = min(max(distance_minor_uv.x, distance_dash_uv.y),
                        max(distance_minor_uv.y, distance_dash_uv.x));
    float a_minor = 1.0;
    if(d_minor > 0) {
        a_minor = d_minor;
        a_minor = exp(-a_minor*a_minor*2);
    }

    fragment_color.rgb = mix(vec3(1.0), vec3(0.7), max(a_major, a_minor));
    fragment_color.a = 1.0;
}
