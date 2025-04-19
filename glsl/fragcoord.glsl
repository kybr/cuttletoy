uniform float u_time;
uniform vec2 u_size;
void main() {
  vec2 pixel = gl_FragCoord.xy;
  if (pixel.x > 100.0) {
    discard;
  }
  if (pixel.y > 100.0) {
    discard;
  }
  gl_FragColor = vec4(1);
}