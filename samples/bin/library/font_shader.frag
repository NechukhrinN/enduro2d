#version 120

uniform sampler2D u_texture;

varying vec2 v_st0;
varying vec4 v_color;

void main() {
    vec2 st = vec2(v_st0.s, 1.0 - v_st0.t);
    gl_FragColor = texture2D(u_texture, st) * v_color;
}
