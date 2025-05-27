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

// https://hosteng.com/dmdhelp/content/instruction_set/SEG_Hex_BCD_to_7_Segment_Display.htm`

float sdBox(vec2 p, vec2 b) {
  vec2 d = abs(p) - b;
  return smoothstep(0.0, 0.015,
                    length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - 0.04);
}


float a(vec2 p) { return sdBox(p + vec2(0, -0.6), vec2(0.2, 0.01)); }
float b(vec2 p) { return sdBox(p + vec2(-0.3, -0.3), vec2(0.01, 0.2)); }
float c(vec2 p) { return sdBox(p + vec2(-0.3, 0.3), vec2(0.01, 0.2)); }
float d(vec2 p) { return sdBox(p + vec2(0, 0.6), vec2(0.2, 0.01)); }
float e(vec2 p) { return sdBox(p + vec2(0.3, 0.3), vec2(0.01, 0.2)); }
float f(vec2 p) { return sdBox(p + vec2(0.3, -0.3), vec2(0.01, 0.2)); }
float g(vec2 p) { return sdBox(p + vec2(0, 0), vec2(0.2, 0.01)); }

void main() {
  vec2 p = pixel();
  p *= 1.0;
  float z = 0.0;
  int id = int(mod(u_time * 2.1, 16.0));
  if (0 == 1) { }
  else if (id == 0) {
    z = min(a(p), min(b(p), min(c(p), min(d(p), min(e(p), f(p))))));
  }
  else if (id == 1) {
    z = min(b(p), c(p));
  }
  else if (id == 2) {
    z = min(a(p), min(b(p), min(d(p), min(e(p), g(p)))));
  }
  else if (id == 3) {
    z = min(a(p), min(b(p), min(c(p), min(d(p), g(p)))));
  }
  else if (id == 4) {
    z = min(b(p), min(c(p), min(f(p), g(p))));
    // b c f g
  }
  else if (id == 5) {
    z = min(a(p), min(c(p), min(d(p), min(f(p), g(p)))));
    // a c d f g
  }
  else if (id == 6) {
    z = min(a(p), min(c(p), min(d(p), min(e(p), min(f(p), g(p))))));
    // not b
  }
  else if (id == 7) {
    z = min(a(p), min(b(p), c(p)));
    // a b c
  }
  else if (id == 8) {
    z = min(a(p), min(b(p), min(c(p), min(d(p), min(e(p), min(f(p), g(p)))))));
    // a b c d e f g
  }
  else if (id == 9) {
    z = min(a(p), min(b(p), min(c(p), min(f(p), g(p)))));
    // not e or d
  }
  else if (id == 0xA) {
    z = min(a(p), min(b(p), min(c(p), min(e(p), min(f(p), g(p))))));
  }
  else if (id == 0xb) {
    z = min(c(p), min(d(p), min(e(p), min(f(p), g(p)))));
  }
  else if (id == 0xC) {
    z = min(a(p), min(f(p), min(e(p), d(p))));
  }
  else if (id == 0xd) {
    z = min(b(p), min(c(p), min(d(p), min(e(p), g(p)))));
  }
  else if (id == 0xE) {
    z = min(a(p), min(d(p), min(e(p), min(f(p), g(p)))));
  }
  else if (id == 0xF) {
    z = min(a(p), min(e(p), min(f(p), g(p))));
  }
  else {
    discard;
  }
  gl_FragColor = vec4(vec3(z), 1.0);
}