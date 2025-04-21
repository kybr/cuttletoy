

uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

#define M_PI 3.1415926535897932384626433832795

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec2 to_polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta / M_PI);
}

// XXX this one is not very good...

void main() {
  vec3 a = hsv2rgb(vec3(to_polar(u_analog_left.xy).xy, u_analog_left.z));
  vec3 b = hsv2rgb(vec3(to_polar(u_analog_right.xy).xy, u_analog_right.z));
  gl_FragColor = mix(vec4(a, 1.0), vec4(b, 1.0), 0.5);
}