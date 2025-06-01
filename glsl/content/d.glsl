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
    p += vec2(0.5);
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

float random(vec2 st) {
  return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 53758.5453123 + 10.0 * sin(u_time / 150.0));
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

void main() {
  vec2 st = pixel();
  vec2 pos = vec2(st * 1.0);
  float n = noise(pos);
  n = floor(n * 20.0);
  if (n == 3.0) {
    gl_FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
    return;
  }
  n = mod(n + 0.01, 2.0);
  if (n < 0.5) {
    discard;
    return;
  }
  gl_FragColor = vec4(vec3(n), 1.0);
}