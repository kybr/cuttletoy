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

void main(void) {
  vec2 uv = pixel();
  uv = rotate(uv, u_time / 3.0);

  float cr = uv.x;
  float ci = uv.y;
  float ar = cr;
  float ai = ci;
  float tr, ti;
  float p = 0.0;
  int i = 0;
  #define N 11 // 19 crashes the pi
  for (int i2 = 1; i2 < N; i2++) {
    tr = ar * ar - ai * ai + cr;
    ti = 2.0 * ar * ai + ci;
    p = tr * tr + ti * ti;
    ar = tr;
    ai = ti;
    if (p > float(N)) {
      i = i2;
      break;
    }
  }
  gl_FragColor = vec4(float(i) * 0.0625, 0, 0, 1);
}