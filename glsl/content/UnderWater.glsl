
uniform float u_time;
uniform vec2 u_size;
uniform vec3 u_screen;
uniform vec3 u_analog_left;
uniform vec3 u_analog_right;
uniform vec2 u_hat;
uniform vec4 u_button;
uniform vec4 u_random;

#define M_PI 3.1415926535897932384626433832795

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

vec2 rot2D (vec2 v, float a){
    float c = cos(M_PI * 2.0 * a);
    float s = sin(M_PI * 2.0 * a);
    return vec2(c*v.x + s*v.y, -s*v.x + c*v.y);

}

float Value3D( vec3 P )
{
    //  https://github.com/BrianSharpe/Wombat/blob/master/Value3D.glsl

    // establish our grid cell and unit position
    vec3 Pi = floor(P);
    vec3 Pf = P - Pi;
    vec3 Pf_min1 = Pf - 1.0;

    // clamp the domain
    Pi.xyz = Pi.xyz - floor(Pi.xyz * ( 1.0 / 69.0 )) * 69.0;
    vec3 Pi_inc1 = step( Pi, vec3( 69.0 - 1.5 ) ) * ( Pi + 1.0 );

    // calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi_inc1.xy ) + vec2( 50.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec2 hash_mod = vec2( 1.0 / ( 635.298681 + vec2( Pi.z, Pi_inc1.z ) * 48.500388 ) );
    vec4 hash_lowz = fract( Pt * hash_mod.xxxx );
    vec4 hash_highz = fract( Pt * hash_mod.yyyy );

    //	blend the results and return
    vec3 blend = Pf * Pf * Pf * (Pf * (Pf * 6.0 - 15.0) + 10.0);
    vec4 res0 = mix( hash_lowz, hash_highz, blend.z );
    vec4 blend2 = vec4( blend.xy, vec2( 1.0 - blend.xy ) );
    return dot( res0, blend2.zxzx * blend2.wwyy );
}
void main() {

    vec2 p = pixel();
    float t = 2.0*M_PI*u_time;

    vec2 uv = p * (1.5 - 0.5*cos(t*.002635)) + vec2(0.5);  
    float mo = floor(mod(p.y*u_size.x, 124.0))/124.0;
    float vo = floor(mod(p.x*u_size.x, 224.0))/224.0;
   
    vec2 vuv = uv + vec2(0.5*vo*sin(t*0.007353 + 1.23423), 
               0.5*mo*cos(t*0.00836353 + 12.2533423));
    float mixer =  sin(t* 0.016635 + 1.4325)*sin(t*.012534 + 0.4514);
    mixer *= mixer*mixer*mixer;          
    uv = mix(uv, vuv, mixer);

    //float result1 = Value4D(vec4(uv.xy + vec2(u_time*0.00243 + 1.432, -u_time*0.006344 + 4.3513), uv.xy + vec2(u_time*0.005324 + 443.234 , u_time*.0014543)));
    float result1 = Value3D(vec3(uv.x + t*0.00143 + 1.432, uv.y + -t*0.003344 + 4.3513, uv.x + t*0.002324 + 443.234 ));
    float smo1 = smoothstep(0.2, 0.9, result1);
    if (smo1 < .3) discard;
    vec3 color = smo1 * (1.0 + 0.5 * vec3(
        0.5 + 0.5*sin(t * 0.01532 + 3.2312 * length(uv)),
        0.0, //.25 + 0.25*cos(t * 0.053243 + 1.243 + 5.4234 * length(uv)),
        0.5 + 0.5*sin(t * 0.04542 + 2.5635 + 3.4132 * length(uv))
    ));
    
    float spec = smoothstep(0.9, 2.0, result1);
    color += spec;
    color *= color*color*color;
    gl_FragColor = vec4(color, 1.0);
    
 }