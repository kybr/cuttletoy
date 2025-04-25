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

int state = 0;
float rnd() {
  state += 12345;
  state *= 1103515245;  // overflow
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

  pixel += vec2(0.1);  // offset
  pixel *= 0.7;        // zoom

  vec3 color = vec3(1.0);

  #define N 11
  int i = 0;
  float t = 0.0;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 1;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 2;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 3;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 4;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }

  // just this much is enough to make the pi very slow;
  
  /*

  // leave in the rest and the pi crashes

  i = 5;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 6;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 7;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 8;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 9;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  i = 10;
  t = float(i + 1) / float(N);
  if (distance(pixel, pol2cart(vec2(t * (1.0 - t), t * sin(u_time * 0.3)))) < 0.05 + t) {
    color = 1.0 - vec3(mod(float(i), 2.0));
    gl_FragColor = vec4(vec3(color), 1.0);
    return;
  }
  */


  gl_FragColor = vec4(vec3(0.0), 1.0); 
}