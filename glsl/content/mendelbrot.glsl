uniform vec2 u_size;
#define N 16
void main(void) {
  float cr = 4.0 * (gl_FragCoord.x - u_size.x / 2.0) / u_size.x;
  float ci = 3.0 * (gl_FragCoord.y - u_size.y / 2.0) / u_size.y;
  float ar = cr;
  float ai = ci;
  int i = 0;
  for (i = 1; i < N; ++i) {
    float tr = ar * ar - ai * ai + cr;
    float ti = 2.0 * ar * ai + ci;
    float p = tr * tr + ti * ti;
    ar = tr;
    ai = ti;
    if (p > 16.0) {
      break;
    }
  }
  gl_FragColor = vec4(float(i) / float(N - 1), 0, 0, 1);
}