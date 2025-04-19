uniform float u_time;
uniform vec2 u_size;

#define M_PI 3.1415926535897932384626433832795

vec2 car2polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta);
}

vec2 rotate(vec2 v, float a) {
  float s = sin(a);
  float c = cos(a);
  mat2 m = mat2(c, s, -s, c);
  return m * v;
}

void main() {
  vec2 p = gl_FragCoord.xy / u_size;
  vec2 uv = p * 2.0 - 1.0;
  uv.x *= u_size.x / u_size.y;

  uv = rotate(uv, u_time / 3.0);

  vec2 polar = car2polar(uv);
  float a = floor(7.0 * polar.y / M_PI);
  float b = floor(5.0 * polar.x);
  // float x = mod(a + b, 2.0);
  float x = mod(a + b, mod(u_time, 5.0));

  if (x < 0.001 && (b == 3.0)) {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    return;
  }

  gl_FragColor = vec4(x);
}
