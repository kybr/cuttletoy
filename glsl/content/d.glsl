uniform float u_time;
uniform vec2 u_size;

float random(vec2 st) {
  return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123 + 5.0 * sin(u_time / 100.0));
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
  vec2 st = gl_FragCoord.xy / u_size;
  vec2 pos = vec2(st * 2.0);
  float n = noise(pos);
  n = floor(n * 30.0);
  if (n == 2.0) {
    gl_FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
    return;
  }
  n = mod(n, 2.0);
  gl_FragColor = vec4(vec3(n), 1.0);
}
