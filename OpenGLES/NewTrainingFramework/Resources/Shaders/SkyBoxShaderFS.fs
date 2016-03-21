precision mediump float;

//TASK8 CubeTextures
uniform samplerCube u_s_Qtexture0;
varying vec3		v_posL;

void main(){
	vec4 final=textureCube(u_s_Qtexture0, v_posL);
	final.a=1.0;
	gl_FragColor=final;
}