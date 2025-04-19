uniform float u_time;
uniform vec2 u_size;

vec2 toPolar(vec2 xy) {
  float dist = length(xy);
  float angle = atan(xy.x, xy.y);
  return vec2(angle / 3.14, dist);
}

void main() {
  // Normalized pixel coordinates (from 0 to 1)
  vec2 uv = gl_FragCoord.xy / u_size.xy;
  uv.x *= u_size.x / u_size.y;

  // Polar coordinates
  vec2 ad = toPolar(vec2(uv.x - 0.5, uv.y - 0.5));

  // Time varying pixel color
  vec3 color = 0.5 + 0.5 * cos(u_time + ad.yyy + vec3(0, 2, 4));
  //vec3 inv = color * vec3(-1.0) + vec3(1.0);

  float pstripe =
      smoothstep(0.49, 0.5, fract((ad.x + ad.y + u_time * -0.125) * 4.0));

  vec3 invstripe = (color * vec3(-1.0) + vec3(1.0)) * (pstripe - 1.0) * -1.0;
  color = color * vec3(pstripe);
  color = color + invstripe;

  // circle mask
  // float pct = distance(uv,vec2(0.5));
  // color = color * vec3(1.0 - smoothstep(0.49,0.5,pct));

  // Output to screen
  gl_FragColor = vec4(color, 1.0);
}