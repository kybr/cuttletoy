uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

#define M_PI 3.1415926535897932384626433832795

float Cellular2D( vec2 P )
{
    //  https://github.com/BrianSharpe/Wombat/blob/master/Cellular2D.glsl

    //  establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec2 Pf = P - Pi;

    //  calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    //  generate the 4 points
    hash_x = hash_x * 2.0 - 1.0;
    hash_y = hash_y * 2.0 - 1.0;
    const float JITTER_WINDOW = 0.25;   // 0.25 will guarentee no artifacts
    hash_x = ( ( hash_x * hash_x * hash_x ) - sign( hash_x ) ) * JITTER_WINDOW + vec4( 0.0, 1.0, 0.0, 1.0 );
    hash_y = ( ( hash_y * hash_y * hash_y ) - sign( hash_y ) ) * JITTER_WINDOW + vec4( 0.0, 0.0, 1.0, 1.0 );

    //  return the closest squared distance
    vec4 dx = Pf.xxxx - hash_x;
    vec4 dy = Pf.yyyy - hash_y;
    vec4 d = dx * dx + dy * dy;
    d.xy = min(d.xy, d.zw);
    return min(d.x, d.y) * ( 1.0 / 1.125 ); // return a value scaled to 0.0->1.0
}

vec2 pixel() {
    vec2 p = gl_FragCoord.xy + vec2(0.5);
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
    
    float nz1 = Cellular2D(p*0.2 + u_time*.0421);
    
    float ocean = step( p.y + nz1*0.15, 0.3); 
    float sky = 1.0 - ocean;
    float path = fract(u_time*0.001212);
    float sunmask = step(path, 0.5);
    float sunpath = sunmask*path * 2.0;
    float sinpath = sin(2.0*M_PI*path);
    float sunpath2 = sinpath*sunmask;//2.0*(step(sunpath, 0.5)*sunpath +  step(0.5, sunpath)*(1.0 - sunpath));
    float moonmask = step(0.5, path);
    float moonpath = (moonmask*path - 0.5)*2.0;
    float moonpath2 = abs(sinpath)*moonmask;

    float sunX = -2.5 + 5.0*sunpath;
    float sun = smoothstep(0.8, 0.0, length(vec2(sunX, 0.2 + 1.0*sunpath2) - p));

    float moonX = -2.5 + 5.0*moonpath;
    float moon = smoothstep(0.8, 0.0, length(vec2(moonX, 0.2 + 0.7*moonpath2) - p));
   
   
    float rays = smoothstep(-1.0, 2.0, p.x*2.0 + (1.0 - p.y))*smoothstep(0.0,2.0,  1.0 - p.x*2.0  + (1.0 - p.y) );
    rays *= ocean;
    p *= 1.0 - 2.0*ocean*(p.y + 0.1);
    p *= 1.0 - 0.8*ocean;
    p += vec2(0.056345*u_time*ocean, .08324*u_time*ocean);
    
    p *= 1.0 - .9*sky;
    p  += vec2(-.03425*u_time*sky, -.01*u_time*sky);

    float nz2 = Cellular2D(p);
    float sea = nz2*ocean + ocean*nz1;
    float air = nz2*sky;
    //sea *= SimplexPerlin2D(p + vec2(13.4325, 22.5342))*2.0;
    vec4 color = rays*0.2*(1.0 - moonpath2) + vec4(
              (air + 0.5*sky)*(1.0-moonpath2) + 1.5*sun + 1.5*sunpath2*sun - sky*sunpath2 + 1.0*moon,
              0.12 + 0.7*sea*(1.0 - 1.6*moonpath2) + 0.8*sun + 0.25*sunpath2*sun + .5*sky*sunpath2 + 1.0*moon,
              0.142 + 0.65*sea*(1.0 - 1.5*moonpath2) + 0.6*air + 0.5*sky*sunpath2 + 1.0*moon,
              1.0);
    //if (length(color.xyz)<0.4 && sky>0.0) discard;
    gl_FragColor =  vec4(color);
}