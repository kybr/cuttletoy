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

  vec2 uv = pixel() * 2.0;
  uv = rotate(uv, u_time / 3.0);

  vec2 polar = car2polar(uv);
  float a = floor(7.0 * polar.y / M_PI);
  float b = floor(5.0 * polar.x);
  //float x = mod(a + b, 2.0);
  float x = mod(a + b + 0.01, mod(u_time, 5.0));

  if (x < 0.001 && (b == 3.0)) {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    return;
  }

  gl_FragColor = vec4(vec3(x), 1.0);
}
