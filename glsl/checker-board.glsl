uniform float u_time;
uniform vec2 u_size;
void main() {
  float size = 120.0;
  vec2 position = floor(gl_FragCoord.xy / size);
  float pattern = mod(position.x + mod(position.y, 2.0), 2.0);
  gl_FragColor = pattern * vec4(1.0);
}
