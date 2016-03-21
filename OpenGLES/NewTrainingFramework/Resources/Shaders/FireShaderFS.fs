precision mediump float;

varying vec2 v_uv;

//TASK7 - MultiTexturing
uniform sampler2D u_s_texture0;
uniform sampler2D u_s_texture1;
uniform sampler2D u_s_texture2;
uniform		float u_time;

void main(){
	float dMax = 0.1;
	vec2 disp = texture2D(u_s_texture2, vec2(v_uv.x, v_uv.y + u_time)).xy;
	vec2 offset = (2.0 * disp -1.0) * dMax;
	vec4 fireColor = texture2D(u_s_texture0,  v_uv + offset);
	vec4 AlphaValue = texture2D(u_s_texture1, v_uv);
	gl_FragColor  = fireColor *(1.0,  1.0, 1.0, AlphaValue.r);
}