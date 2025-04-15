uniform float u_time;
uniform vec2 u_size;
void main() {
  vec2 pixel = gl_FragCoord.xy;
  float N = 100.0;
  if (pixel.y < N) {
    discard;
  }
  if (pixel.x < N) {
    discard;
  }
  if (pixel.y > u_size.y - N) {
    discard;
  }
  if (pixel.x > u_size.x - N) {
    discard;
  }
  vec2 p = mod(pixel, vec2(N));
  gl_FragColor = vec4(p, 1, 1);
}