uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

#define M_PI 3.1415926535897932384626433832795

// bitwise operators not available...
//
// int xorshift(int value) {
//   // Xorshift*32
//   // Based on George Marsaglia's work: http://www.jstatsoft.org/v08/i14/paper
//   value ^= value << 13;
//   value ^= value >> 17;
//   value ^= value << 5;
//   return value;
// }


int state = 0;

float rnd() {
  state += 12345;
  state *= 1103515245;  // overflow
  return float(state) / 4294967296.0;
}

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

#define M_PI 3.1415926535897932384626433832795

float random(vec2 st) {
  return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
  // return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123 + 5.0 *
  // sin(u_time / 100.0));
}

float noise(vec2 st) {
  vec2 i = floor(st);
  vec2 f = fract(st);
  float a = random(i);
  float b = random(i + vec2(1.0, 0.0));
  float c = random(i + vec2(0.0, 1.0));
  float d = random(i + vec2(1.0, 1.0));
  vec2 u = f * f * (3.0 - 2.0 * f);
  return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

vec2 car2polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta / M_PI);
}

void main() {
  gl_FragColor = vec4(vec3(1.0), 1.0);
  vec2 p = pixel();
  float n = noise(
      vec2(noise(p * 10000.0 + u_time * 0.01), noise(car2polar(p * 10000.0 + u_time * 0.01))));
  if (n < 0.5) {
    discard;
  }
}
