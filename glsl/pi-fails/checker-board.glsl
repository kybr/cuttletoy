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
  // XXX note the column of white pixels on the left
  // why is this happening? this column of white does
  // not appear on the desktop. there is something about
  // the pi GLSL/OpenGL implementation that is causing
  // this to happen. we can "fix" this by shifting
  // the fragment coord by 1.0, but that is not a real
  // fix
  fc.x += 1.0;
  //fc.x *= 1.0;
  vec2 position = fc / u_size;
  position.x *= u_size.x / u_size.y; // square the pattern
  position *= 5.0;

  // XXX also this... we must negate the or the checkerboard
  // pattern does not appear!! try uncommenting this line:
  position.x *= -1.0;
  // XXX is this a bug in the mod or floor functions?

  position = floor(position);
  float pattern = mod(position.x + mod(position.y, 2.0), 2.0);
  gl_FragColor = vec4(vec3(pattern), 1.0);
}
