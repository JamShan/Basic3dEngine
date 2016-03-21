attribute	vec3	a_posL;

//TASK2
uniform	mat4	u_VWPmatrix;

//TASK8
uniform		mat4	u_Wmatrix;
uniform		vec3	u_camera;
attribute	vec3	a_norm;
varying		vec3	v_reflect;
varying		vec3	v_norm_W;
varying		vec3	v_pos_W;

void main(){
	vec4 posL = vec4(a_posL, 1.0);
	v_norm_W=(u_Wmatrix*vec4(a_norm,0.0)).xyz;
	v_pos_W = (u_Wmatrix*posL).xyz;
	gl_Position = u_VWPmatrix * posL;
	
	//moved here to optimize computing resources
	vec3 v_toEye = u_camera - v_pos_W;
	v_reflect = reflect(normalize(-v_toEye),normalize(v_norm_W));
	
}
   
