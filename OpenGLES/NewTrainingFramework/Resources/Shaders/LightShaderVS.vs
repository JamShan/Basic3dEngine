attribute	vec3	a_posL;

//TASK2
uniform		mat4	u_VWPmatrix;

varying		vec2	v_uv;
attribute	vec2	a_uv;

//TASK8
uniform		mat4	u_Wmatrix;
attribute	vec3	a_norm;
varying		vec3	v_norm_W;
varying		vec3	v_pos_W;

void main(){
	v_uv = a_uv;
	
	vec4 posL = vec4(a_posL, 1.0);
	v_norm_W= (u_Wmatrix*vec4(a_norm,0.0)).xyz;
	v_pos_W = (u_Wmatrix*posL).xyz;
	gl_Position = u_VWPmatrix * posL;
	
}
   
