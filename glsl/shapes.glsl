uniform vec2 u_size;
uniform float u_time;

float opUnion(float d1, float d2) { return min(d1, d2); }
float opSubtraction(float d1, float d2) { return max(-d1, d2); }
float opIntersection(float d1, float d2) { return max(d1, d2); }
float opXor(float d1, float d2) { return max(min(d1, d2), -max(d1, d2)); }

float opOnion(in float sdf, in float thickness) { return abs(sdf) - thickness; }

float opSmoothUnion(float d1, float d2, float k) {
  float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
  return mix(d2, d1, h) - k * h * (1.0 - h);
}

float opSmoothSubtraction(float d1, float d2, float k) {
  float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
  return mix(d2, -d1, h) + k * h * (1.0 - h);
}

float opSmoothIntersection(float d1, float d2, float k) {
  float h = clamp(0.5 - 0.5 * (d2 - d1) / k, 0.0, 1.0);
  return mix(d2, d1, h) + k * h * (1.0 - h);
}

float sdCircle(vec2 p, in float r) { return length(p) - r; }

float sdBox(in vec2 p, in vec2 b) {
  vec2 d = abs(p) - b;
  return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sdOrientedBox(in vec2 p, in vec2 a, in vec2 b, float th) {
  float l = length(b - a);
  vec2 d = (b - a) / l;
  vec2 q = (p - (a + b) * 0.5);
  q = mat2(d.x, -d.y, d.y, d.x) * q;
  q = abs(q) - vec2(l, th) * 0.5;
  return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0);
}

// https://www.shadertoy.com/view/3ltSW2

void main() {
  vec2 p = (2.0 * gl_FragCoord.xy - u_size.xy) / u_size.y;

  //float d = opOnion(sdCircle(p, fract(u_time / 100.0)), 0.2);

  float d = sdOrientedBox(p, vec2(sin(u_time), cos(u_time)), vec2(0.5, -0.5), 0.1);
  d = opSmoothUnion(d, sdCircle(p, 0.2), 0.1);

  vec3 color = (d > 0.0) ? vec3(0.1, 0.6, 0.7) : vec3(0.65, 0.85, 1.0);
  color *= 1.0 - exp(-6.0*abs(d));
  color *= 0.8 + 0.2*cos(150.0*d);
  color = mix(color, vec3(1.0), 1.0 - smoothstep(0.0, 0.01, abs(d)));

  gl_FragColor = vec4(color, 1.0);
}
