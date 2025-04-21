

uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

void main() {
  float color = 0.0;

  vec2 p = gl_FragCoord.xy;
  if (u_screen.z < 0.0) {
    p /= u_size;
  } else {
    p += vec2(u_screen.x * 1920.0, (3.0 - u_screen.y) * 1080.0);
    p /= vec2(4.0 * 1920.0, 4.0 * 1080.0);
  }
  p -= vec2(0.5, 0.5);
  p.x *= u_size.x / u_size.y;  // square the pattern
  p += vec2(sin(u_time) * 0.3, cos(u_time) * 0.3);

  color = length(p) < 0.09 ? 1.0 : 0.0;

  gl_FragColor = vec4(vec3(color), 1.0);
}