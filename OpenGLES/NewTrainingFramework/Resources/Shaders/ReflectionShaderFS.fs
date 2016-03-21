precision mediump float;

//TASK8 Reflect CubeTexture
uniform 	samplerCube	u_s_Qtexture0;
varying 	vec3		v_reflect;

void main(){
	gl_FragColor=textureCube(u_s_Qtexture0, v_reflect);
}