uniform float u_time;
uniform vec2 u_size;

vec2 rotate(vec2 v, float a) {
  float s = sin(a);
  float c = cos(a);
  mat2 m = mat2(c, s, -s, c);
  return m * v;
}

#define M_PI 3.1415926535897932384626433832795

vec2 car2polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta / M_PI);
}

float circle(vec2 p, float r, float t) {
  float l = r - length(p);
  if (abs(l) < t) {
    return 1.0;
  }
  return 0.0;
}

void main() {
  vec4 data[3];
  data[0] = vec4(0.0, 0.0, 0.1, 0.01);
  data[1] = vec4(0.1, 0.0, 0.2, 0.01);
  data[2] = vec4(0.2, 0.0, 0.3, 0.01);

  vec2 pixel = (2.0 * gl_FragCoord.xy - u_size.xy) / u_size.y;
  pixel = rotate(pixel, u_time);
  pixel = car2polar(pixel); // pixel.x is radius, pixel.y is angle

  float color = 1.0;

  for (int i = 0; i < 3; i++) {
  }
  if (circle(pixel + vec2(0.1), 0.7, 0.01) > 0.5) {
    color = 0.0;
  }
  if (circle(pixel + vec2(-0.3, 0.2), 0.3, 0.01) > 0.5) {
    color = 0.0;
  }
  if (circle(pixel + vec2(-0.1, 0.05), 0.1, 0.01) > 0.5) {
    color = 0.0;
  }

  gl_FragColor = vec4(vec3(color), 1.0);
}