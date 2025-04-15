uniform vec2 u_size;
uniform float u_time;

float sdCircle(vec2 p, in float r) {
    return length(p)-r;
}

// https://www.shadertoy.com/view/3ltSW2

void main() {
	vec2 p = (2.0 * gl_FragCoord.xy - u_size.xy) / u_size.y;

	float d = sdCircle(p, fract(u_time / 100.0));

    vec3 color = (d > 0.0) ? vec3(0.1, 0.6, 0.7) : vec3(0.65, 0.85, 1.0);
    //color *= 1.0 - exp(-6.0*abs(d));
	//color *= 0.8 + 0.2*cos(150.0*d);
	color = mix(color, vec3(1.0), 1.0 - smoothstep(0.0, 0.01, abs(d)));

	gl_FragColor = vec4(color, 1.0);
}