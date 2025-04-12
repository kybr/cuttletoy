uniform float time;
uniform vec2 size;
void main() {  //
  vec2 uv = gl_FragCoord.xy / size;
  gl_FragColor = vec4(vec3(uv.x, fract(time), 0.0), 1.0);
}
