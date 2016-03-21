precision mediump float;

varying vec2 v_uv;
varying vec2 v_uvTiling;

//TASK7 - MultiTexturing
uniform sampler2D u_s_texture0;
uniform sampler2D u_s_texture1;
uniform sampler2D u_s_texture2;
uniform sampler2D u_s_texture3;

//TASK7 - Fog
uniform vec3	u_camera;
uniform vec4	u_fogColor;
uniform float	u_fogStart;
uniform float	u_fogRange;
varying vec4	v_posWorld;

void main(){
	//MultiTexturing
	vec4 Map		= texture2D(u_s_texture0, v_uv);
	vec4 dirt		= texture2D(u_s_texture1, v_uvTiling, -1.5);
	vec4 grass		= texture2D(u_s_texture2, v_uvTiling, -1.5);
	vec4 rock		= texture2D(u_s_texture3, v_uvTiling, -1.5);
	vec4 terrain	= (Map.r * rock + Map.g * grass + Map.b * dirt) / (Map.r+ Map.g+ Map.b);	
	
	//Fog
//if(u_camera.x == 0.0){
	float distCam2Frag = distance(u_camera, v_posWorld.xyz);
	float lerpValue	=clamp((distCam2Frag - u_fogStart)/u_fogRange, 0.0, 1.0);
	vec4 final = mix(terrain,u_fogColor,lerpValue);
	final.a=1.0;
	
	gl_FragColor	= final;
//}else{
//	gl_FragColor	= rock;
//}

}