uniform float u_time;
uniform vec2 u_size;
void main() {
  vec2 uv = gl_FragCoord.xy / u_size;
  gl_FragColor = vec4(vec3(uv.x, 1.0 - fract(2.0 * u_time * 1.0), 0.0), 1.0);
}