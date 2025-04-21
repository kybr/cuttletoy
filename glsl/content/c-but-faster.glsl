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

vec2 car2polar(vec2 p) {
  float r = length(p);
  float theta = atan(p.y, p.x);
  return vec2(r, theta);
}

void main() {
  vec2 p = pixel();
  p = rotate(p, u_time);
  vec2 polar = car2polar(p);

  // https://thebookofshaders.com/13/
  float x = polar.x;
  float amplitude = 2.;
  float frequency = 1.;
  float y = sin(x * frequency);
  float t = 0.01 * (-u_time * 30.0);
  y += sin(x * frequency * 2.1 + t) * 4.5;
  y += sin(x * frequency * 1.72 + t * 1.121) * 4.0;
  y += sin(x * frequency * 2.221 + t * 0.437) * 5.0;
  y += sin(x * frequency * 3.1122 + t * 4.269) * 2.5;
  y *= amplitude * 0.06;

  p += vec2(0.1 * y);
  p = car2polar(p);

  float color = 1.0;
  p.x = sin(1.0 * p.x) + p.x;
  float ring = floor(p.x * 20.0);
  if (ring == 2.0 * floor(mod(u_time * 3.0, 30.0))) {
    gl_FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
    return;
  }
  color = mod(ring, 2.0);
  gl_FragColor = vec4(vec3(color), 1.0);
}