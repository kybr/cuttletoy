uniform float u_time;
uniform vec2 u_size;
// uniform vec4 u_parameter;

#define M_PI 3.1415926535897932384626433832795

vec2 car2polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta / M_PI);
}

void main() {
  vec2 uv = gl_FragCoord.xy / u_size.xy;
  uv.x *= u_size.x / u_size.y;

  vec2 polar = car2polar(vec2(uv.x - 0.5, uv.y - 0.5));

  float strip = floor(polar.y * 10.0 + polar.x * 10.0 - u_time / 1.0);
  float color = mod(strip, 2.0);
  if (mod(strip, 10.0) == 2.0) {
    gl_FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
    return;
  }
  
  gl_FragColor = vec4(vec3(color), 1.0);
}