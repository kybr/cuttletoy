uniform float u_time;
uniform vec2 u_size;

void main() {
  vec2 p = gl_FragCoord.xy / u_size;
  vec2 uv = p * 2.0 - 1.0;
  uv.x *= u_size.x / u_size.y;
  float d = length(uv);
  if (d < 0.5) {
    gl_FragColor = vec4(0);
  } else {
    gl_FragColor = vec4(1);
  }
}