uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;

vec2 rotate(vec2 v, float a) {
  float s = sin(a);
  float c = cos(a);
  mat2 m = mat2(c, s, -s, c);
  return m * v;
}

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
  vec2 p = gl_FragCoord.xy / u_size;
  float pi = (u_screen.z - 1.0) / 16.0;
  p -= vec2(0.5);
  p.x *= u_size.x / u_size.y;
  p *= 1.0 + 0.1 * sin(u_time + pi);
  p = rotate(p, u_time + pi * 10.0);
  float color = 1.0;

  if (distance(p, vec2(0, 0)) < 0.0001) {
    color = 0.00001;
  } else

      if (distance(p, vec2(0.02987906959880472, 0.014389001552368)) <
          0.07152857142857143) {
    color = 1.00001;
  } else

      if (distance(p, vec2(0.0381728450117592, 0.04786723362049162)) <
          0.14295714285714284) {
    color = 0.00001;
  } else

      if (distance(p, vec2(0.018732629644281573, 0.08207301301530658)) <
          0.21438571428571426) {
    color = 1.00001;
  } else

      if (distance(p, vec2(-0.022706217750644322, 0.09948244001855343)) <
          0.2858142857142857) {
    color = 0.00001;
  } else

      if (distance(p, vec2(-0.07157408439704847, 0.0897510630384218)) <
          0.35724285714285714) {
    color = 1.00001;
  } else

      if (distance(p, vec2(-0.11032271851866354, 0.053128621116435695)) <
          0.42867142857142854) {
    color = 0.00001;
  } else

      if (distance(p, vec2(-0.125, 1.5308084989341915e-17)) < 0.5001) {
    color = 1.00001;
  } else

      if (distance(p, vec2(-0.11032271851866357, -0.053128621116435674)) <
          0.5715285714285714) {
    color = 0.00001;
  } else

      if (distance(p, vec2(-0.0715740843970485, -0.08975106303842177)) <
          0.6429571428571429) {
    color = 1.00001;
  } else

      if (distance(p, vec2(-0.022706217750644346, -0.09948244001855343)) <
          0.7143857142857143) {
    color = 0.00001;
  } else

      if (distance(p, vec2(0.018732629644281556, -0.08207301301530659)) <
          0.7858142857142857) {
    color = 1.00001;
  } else

      if (distance(p, vec2(0.038172845011759195, -0.04786723362049164)) <
          0.8572428571428571) {
    color = 0.00001;
  } else

      if (distance(p, vec2(0.0298790695988047, -0.014389001552368)) <
          0.9286714285714286) {
    color = 1.00001;
  } else

      if (distance(p, vec2(0, 0)) < 1.0001) {
    color = 0.00001;
  } else

  {
    color = 0.0;
  }

  if (color < 0.5) {
    discard;
    return;
  }

  vec3 rgb = hsv2rgb(vec3(pi, fract(u_time / 100.0), 1.0));
  gl_FragColor = vec4(rgb, 1.0);

  //gl_FragColor = vec4(vec3(color), 1.0);
}