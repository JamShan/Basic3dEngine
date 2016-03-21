attribute	vec3	a_posL;
varying		vec3	v_posL;

//TASK2
uniform		mat4	u_VWPmatrix;

void main(){
	vec4 posL = vec4(a_posL, 1.0);
	v_posL = a_posL;
	gl_Position = u_VWPmatrix * posL;//TASK2	
}
   