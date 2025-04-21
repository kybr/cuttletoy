uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

void main() {
  vec2 pixel = fract(gl_FragCoord.xy);
  // fragcoord are at (0.5, 0.5)
  float color = pixel.y == 0.5 ? 1.0 : 0.0;
  gl_FragColor = vec4(vec3(color), 1);
}