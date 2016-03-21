precision mediump float;

float SPECULAR_POWER = 20.0;

varying vec2		v_uv;
uniform sampler2D	u_s_texture0;
uniform sampler2D	u_s_texture1;

varying	vec3		v_norm_W;
varying	vec3		v_tangent_W;
varying	vec3		v_binorm_W;
varying	vec3		v_pos_W;

uniform	int			u_noLights;
uniform vec3		u_ambCol;
uniform float		u_ambWei;

uniform vec3		u_ligDirAr[10];
uniform vec3		u_ligColAr[10];
uniform int			u_ligTypAr[10];

void main(){
	vec4 texture		 = texture2D(u_s_texture0, v_uv);
	vec3 DiffuseTotal	 = vec3(0.0,0.0,0.0);
	vec3 SpecularTotal	 = vec3(0.0,0.0,0.0);
	vec3 diffPosDir;
	vec3 reflectDir;
	float DiffuseIntensity	 = 0.0;
	float SpecularIntensity  = 0.0;
	
	vec3 normal = texture2D(u_s_texture1,v_uv).xyz;
    mat3 TBN = mat3(normalize(v_tangent_W),normalize(v_binorm_W),normalize(v_norm_W));
    vec3 v_norm2 = normalize(TBN * (2.0 * normal - 1.0));
    
	int	i;
	
	for( i=0 ; i < u_noLights; i++){
	
	
		if( u_ligTypAr[i] == 0 ){			//DIR - no possition
			diffPosDir = normalize(u_ligDirAr[i]);
		} else {							//POINT
			diffPosDir = normalize(v_pos_W - u_ligDirAr[i]);
		}		
		DiffuseIntensity = max(dot(v_norm2, -(diffPosDir)), 0.0)/1.0;
		reflectDir = normalize((reflect(diffPosDir, v_norm2)));
		
		DiffuseTotal += u_ligColAr[i] * DiffuseIntensity;
		SpecularIntensity = pow(max(dot(reflectDir, v_norm2),0.0),SPECULAR_POWER);
		SpecularTotal += u_ligColAr[i] * SpecularIntensity;
	}	
	gl_FragColor = vec4(((u_ambCol * u_ambWei + DiffuseTotal * (1.0 - u_ambWei))
						* texture.xyz + SpecularTotal) , texture.w);	
}
