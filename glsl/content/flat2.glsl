uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;


vec2 rotate(vec2 v, float a) {
  float s = sin(a);
  float c = cos(a);
  mat2 m = mat2(c, s, -s, c);
  return m * v;
}

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


void main() {
  vec2 p = pixel();
  p = rotate(p, u_time);
  float color = 1.0;

  if (distance(p, vec2(0.06843882079228888, -0.04436437942668898)) < 0.1197698946343735) {
    color = 0.00001;
  } else

  if (distance(p, vec2(0.05346226282813436, -0.029595056465362545)) < 0.1593646170804277) {
    color = 1.00001;
  } else

  if (distance(p, vec2(0.02144968642284211, -0.061224822578490684)) < 0.2377899959217757) {
    color = 0.00001;
  } else

  if (distance(p, vec2(0.02125830134657916, -0.06066698771002553)) < 0.24796702861785888) {
    color = 1.00001;
  } else

  if (distance(p, vec2(0.02654371017054857, -0.07694799846222619)) < 0.2882989753736183) {
    color = 0.00001;
  } else

  if (distance(p, vec2(0.033866747604267146, -0.10027644429540819)) < 0.34909297551494095) {
    color = 1.00001;
  } else

  if (distance(p, vec2(0.012519381283089229, -0.1145054153067393)) < 0.3784258398227394) {
    color = 0.00001;
  } else

  if (distance(p, vec2(-0.0007407282632805358, -0.05711351378718331)) < 0.46892313980497424) {
    color = 1.00001;
  } else

  if (distance(p, vec2(-0.005077885266577118, -0.05678636201469092)) < 0.4881128268549219) {
    color = 0.00001;
  } else

  if (distance(p, vec2(-0.04324027277520513, -0.10398809991978408)) < 0.5806959382491187) {
    color = 1.00001;
  } else

  if (distance(p, vec2(-0.04558127959040076, -0.09710277522274273)) < 0.6430336768273265) {
    color = 0.00001;
  } else

  if (distance(p, vec2(-0.0498982022749344, -0.1347764731772727)) < 0.7071814202703536) {
    color = 1.00001;
  } else

  if (distance(p, vec2(-0.06057761378061983, -0.13323970860883189)) < 0.7207581359660253) {
    color = 0.00001;
  } else

  if (distance(p, vec2(-0.0803010859494077, -0.08836520863239625)) < 0.7921951591735706) {
    color = 1.00001;
  } else

  if (distance(p, vec2(-0.07884970513578501, -0.15762069572821896)) < 0.8912582715973258) {
    color = 0.00001;
  } else

  if (distance(p, vec2(-0.07444001331758915, -0.14803710008194224)) < 0.9290812138002367) {
    color = 1.00001;
  } else

  if (distance(p, vec2(-0.0393263338618833, -0.09328837085495159)) < 1.0116265946300702) {
    color = 0.00001;
  } else

  if (distance(p, vec2(-0.006987503198003703, -0.07902640422388557)) < 1.0739809330785648) {
    color = 1.00001;
  } else

  if (distance(p, vec2(0.0046227222415272835, -0.10040407831851623)) < 1.130642585968599) {
    color = 0.00001;
  } else

  if (distance(p, vec2(0.022002122891804067, -0.12623055109309902)) < 1.1634280087426307) {
    color = 1.00001;
  } else

  {
    color = 0.0;
  }
  if (color < 0.5) {
    discard;
  }

  gl_FragColor = vec4(vec3(color), 1.0);
}