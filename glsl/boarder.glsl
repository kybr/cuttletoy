uniform float u_time;
uniform vec2 u_size;

void main() {
  vec2 pixel = gl_FragCoord.xy / u_size;
  float f = 0.0;
  const float e = 0.01;
  if (pixel.x < e || pixel.x > 1.0 - e || pixel.y < e || pixel.y > 1.0 - e) {
    f = 1.0;
  }
  gl_FragColor = vec4(vec3(f), 1.0);
}