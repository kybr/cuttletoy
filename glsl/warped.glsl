uniform float u_time;
uniform vec2 u_size;
void main() {
  vec2 pixel = (2.0 * gl_FragCoord.xy - u_size.xy) / u_size.y;
  pixel.x += 0.01 * sin(10.0 * pixel.y) / pixel.y;
  pixel.y += 0.01 * sin(10.0 * pixel.x) / pixel.x;
  vec2 position = floor(10.0 * pixel);
  float pattern = mod(position.x + mod(position.y, 2.0), 2.0);
  gl_FragColor = vec4(vec3(pattern), 1.0);
}