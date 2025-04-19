uniform float u_time;
uniform vec2 u_size;

int state = 0;
float rnd() {
  state += 12345;
  state *= 1103515245; // overflow
  return float(state) / 4294967296.0;
}

vec2 rotate(vec2 v, float a) {
  float s = sin(a);
  float c = cos(a);
  mat2 m = mat2(c, s, -s, c);
  return m * v;
}

#define M_PI 3.1415926535897932384626433832795

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

float within(vec2 p, vec2 c, float r) {
  return (distance(p, c) < r) ? 1.0 : 0.0;
}

void main() {

  vec2 pixel = (2.0 * gl_FragCoord.xy - u_size.xy) / u_size.y;
  pixel = rotate(pixel, u_time);

  pixel += vec2(0.1); // offset
  pixel *= 0.7; // zoom

  vec3 color = vec3(1.0);

  int N = 19;
  float t_ = 0.0;
  for (int i = 0; i < N; i++) {
    float t = float(i) / float(N - 1);
    float radius = 0.05 + t;
    // t * (1.0 - t) ... is an "ah ha" moment
    // it makes the deviation small when the 
    // circles are small or large, but in between
    // they deviate a lot
    float magnitude = t * (1.0 - t);
    float angle = t * sin(u_time * 0.3);
    if (distance(pixel, pol2cart(vec2(magnitude, angle))) < radius) {
        color = 1.0 - vec3(mod(float(i), 2.0));
        if (i == 2 * int(mod(u_time * 3.0, 10.0))) {
            color = vec3(1.0, 0.0, 0.0);
        }
        break;
    }
  }

  if (length(pixel) > 1.1) {
    color = vec3(0.0);
  }

  gl_FragColor = vec4(color, 1.0);
}
