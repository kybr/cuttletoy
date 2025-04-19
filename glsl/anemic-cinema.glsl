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

vec2 pol2cart(vec2 p) {
    return vec2(p.x * cos(p.y * M_PI), p.x * sin(p.y * M_PI));
}

float circle(vec2 p, float r, float t) {
  float l = r - length(p);
  if (abs(l) < t) {
    return 1.0;
  }
  return 0.0;
}

#define N (10)
void main() {
  vec4 data[N];
  data[0] = vec4(0.005,0.1, 0.1, 0.005);
  data[1] = vec4(0.01, 0.2, 0.2, 0.005);
  data[2] = vec4(0.02, 0.3, 0.3, 0.005);
  data[3] = vec4(0.03, 0.4, 0.4, 0.005);
  data[4] = vec4(0.04, 0.5, 0.5, 0.01);
  data[5] = vec4(0.05, 0.6, 0.6, 0.01);
  data[6] = vec4(0.06, 0.7, 0.7, 0.02);
  data[7] = vec4(0.07, 0.8, 0.8, 0.02);
  data[8] = vec4(0.08, 0.9, 0.9, 0.01);
  data[9] = vec4(0.09, 1.0, 1.0, 0.01);

  vec2 pixel = (2.0 * gl_FragCoord.xy - u_size.xy) / u_size.y;
  pixel = rotate(pixel, u_time);
  pixel = car2polar(pixel); // pixel.x is radius, pixel.y is angle

  float color = 1.0;

  for (int i = 0; i < N; i++) {
    vec2 p = car2polar(pol2cart(pixel) + pol2cart(data[i].xy));
    float r = data[i].z;
    float t = data[i].w;
    if (abs(r - p.x) < t) {
        color = 0.0;
    }
  }

  if (pixel.x > 0.9) {
    color = 0.0;
  }

  gl_FragColor = vec4(vec3(color), 1.0);
}
