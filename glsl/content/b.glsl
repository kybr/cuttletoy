uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

// XXX this one seems to work on the pis

vec2 pixel() {
  vec2 p = gl_FragCoord.xy;
  p += vec2(0.5);
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
  return vec2(r, theta / M_PI);
}

void main() {
  vec2 uv = pixel();

  vec2 polar = car2polar(uv);

  float N = 10.0;
  float strip = floor(polar.y * N + polar.x * N - u_time / 1.0);
  float color = mod(strip, 2.0);
  if (mod(strip, N) == 2.0) {
    gl_FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
    return;
  }

  if (color == 0.0) {
    discard;
  }
  
  gl_FragColor = vec4(vec3(color), 1.0);
}