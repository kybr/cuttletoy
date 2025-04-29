uniform vec2 u_size;

void main() {
  vec2 p = gl_FragCoord.xy / u_size;
  p = p * 2.0 - 1.0;
  p.x *= u_size.x / u_size.y;
  if (length(p) < 0.5) {
    gl_FragColor = vec4(1);
  } else {
    discard;
  }
}
