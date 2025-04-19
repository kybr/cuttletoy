uniform float u_time;
uniform vec2 u_size;

#define M_PI 3.1415926535897932384626433832795

vec2 car2polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta);
}

void main() {
  vec2 p = gl_FragCoord.xy / u_size;
  vec2 uv = p * 2.0 - 1.0;
  uv.x *= u_size.x / u_size.y;

  vec2 polar = car2polar(uv);
  if (fract(5.0 * polar.y / M_PI) < 0.5) {
    gl_FragColor = vec4(0);
    return;
  }

  gl_FragColor = vec4(1);
}