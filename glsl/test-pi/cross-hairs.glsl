uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

void main() {
  vec2 pixel = gl_FragCoord.xy;
  float color = 0.0;
  if (int(pixel.x) == int(u_size.x) / 2) {
    color = 1.0;
  }
  if (int(pixel.y) == int(u_size.y) / 2) {
    color = 1.0;
  }
  gl_FragColor = vec4(vec3(color), 1);
}