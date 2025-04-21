

  uniform float u_time;
  uniform vec2 u_size;
  uniform vec3 u_screen;
  uniform vec3 u_analog_left;
  uniform vec3 u_analog_right;
  uniform vec2 u_hat;
  uniform vec4 u_button;
  uniform vec4 u_random;
  
  void main() {
    gl_FragColor = vec4(vec3(fract(u_time / 2.0)), 1.0);
  }