uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
  float pi = (u_screen.z - 1.0) / 16.0;
  float a = 0.5 + sin(u_time * 0.5 + pi * 3.0) * 0.5;
  float b = pow(sin(u_time * 0.1), 2.0);
  gl_FragColor = vec4(hsv2rgb(vec3(a, b, 1.0)), 1.0);
}