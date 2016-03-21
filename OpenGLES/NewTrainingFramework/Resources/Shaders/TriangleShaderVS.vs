attribute vec3 a_posL;

//TASK1
//attribute vec4 a_color;
//varying vec4 v_color;

//TASK2
uniform mat4 u_VWPmatrix;

//TASK5
attribute vec2 a_uv;
varying vec2 v_uv;

void main(){
	vec4 posL = vec4(a_posL, 1.0);
	v_uv = a_uv;
	gl_Position = u_VWPmatrix * posL;//TASK2
}
   