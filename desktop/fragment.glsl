uniform float time;
uniform vec2 size;
void main() {
  vec2 uv = gl_FragCoord.xy / size;
  gl_FragColor = vec4(vec3(uv.x, 1.0 - fract(time / 1.0), 0.0), 1.0);
}
