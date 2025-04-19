uniform float u_time;
uniform vec2 u_size;
float rand(float x) { return fract(sin(x) * 100000.0); }
void main() {
  float index = gl_FragCoord.x + gl_FragCoord.y * u_size.x;
  float r = mix(rand(index / (100.0 * rand(index))),
                rand(1000.0 + index / 1234.0), sin(u_time) / 2.0 + 0.5);
  gl_FragColor = vec4(vec3(r), 1.0);
}
