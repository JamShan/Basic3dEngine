attribute vec3 a_posL;

//TASK2
uniform mat4 u_VWPmatrix;

//TASK5
attribute vec2	a_uv;
varying vec2	v_uv;

//TASK7
varying vec2	v_uvTiling;
uniform mat4	u_Wmatrix;
varying vec4	v_posWorld;//sending obj pos to FS

void main(){
	
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_VWPmatrix * posL;//TASK2
	
	v_uv = a_uv;	
	
	//Task7
	v_uvTiling.x = a_uv.x * 32.0;
	v_uvTiling.y = a_uv.y * 32.0;
	v_posWorld = u_Wmatrix * posL;
	
}
   