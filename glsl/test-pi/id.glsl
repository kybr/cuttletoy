uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

void main() {
  if (u_screen.z < 0.0) {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    return;
  }
  float t = u_screen.z / 16.0;
  gl_FragColor = vec4(vec3(1.0 - t), 1.0);
}