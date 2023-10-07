#version 150

uniform vec4 color;
uniform float line_width;
uniform vec4 view_rect;

in vec2 v_p;
in float line_length;

out vec4 fragment_color;

void main() {
    if (gl_FragCoord.x < view_rect.x ||
            gl_FragCoord.x > view_rect.z ||
            gl_FragCoord.y < view_rect.y ||
            gl_FragCoord.y > view_rect.w) {
        fragment_color = vec4(0.0);
    } else {
        float antialias = 5;
        float d = 0;
        if( v_p.x < 0 )
            d = length(v_p) - line_width + antialias/2.0;
        else if ( v_p.x > line_length )
            d = length(v_p - vec2(line_length, 0.0)) - line_width + antialias/2.0;
        else
            d = abs(v_p.y) - line_width + antialias/2.0;
        if( d < 0)
            fragment_color = color;
        else {
            d = exp(-d*d);
            fragment_color = vec4(color.rgb, d*color.a);
        }
    }


//   fragment_color = color;
}
