uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

void main() {
  vec2 fc = gl_FragCoord.xy;
  //fc.x += 1.0;
  vec2 position = fc / u_size;
  position.x *= u_size.x / u_size.y;
  position *= 10.0;
  position.x *= -1.0;
  position = floor(position);
  float pattern = mod(position.x + mod(position.y, 2.0), 2.0);
  gl_FragColor = vec4(vec3(pattern), 1.0);
}
