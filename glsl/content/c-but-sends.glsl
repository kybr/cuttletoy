
uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

vec2 pixel() {
  vec2 p = gl_FragCoord.xy;
  if (u_screen.z == 0.0) {
    p /= u_size;
  } else {
    float ppi = 93.34;
    float width = 1920.0 + ppi;
    float height = 1080.0 + ppi;
    p += vec2(u_screen.x * width, (3.0 - u_screen.y) * height);
    p += vec2(ppi / 2.0);
    p /= vec2(4.0 * width, 4.0 * height);
  }
  p -= vec2(0.5, 0.5);
  p.x *= u_size.x / u_size.y;  // square the pattern
  p *= 2.0;
  return p;
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

void main() {
  vec2 p = pixel();
  p = rotate(p, u_time);

  vec3 color = vec3(1.0);

  #define N 7 // 7 and we get GL_OUT_OF_MEMORY on the pis
  for (int i = 0; i < N; i++) {
    float t = float(i) / float(N - 1);
    float radius = 0.05 + t;
    float magnitude = t * (1.0 - t);
    float angle = t * sin(u_time * 0.3);
    if (distance(p, pol2cart(vec2(magnitude, angle))) < radius) {
      color = 1.0 - vec3(mod(float(i), 2.0));
      if (i == 2 * int(mod(u_time * 3.0, 10.0))) {
        color = vec3(1.0, 0.0, 0.0);
      }
      break;
    }
  }
  gl_FragColor = vec4(color, 1.0);
}