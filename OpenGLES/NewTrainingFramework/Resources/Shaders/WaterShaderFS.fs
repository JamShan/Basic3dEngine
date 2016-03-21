precision mediump float;

uniform sampler2D	u_s_texture0;
uniform sampler2D	u_s_texture1;
uniform sampler2D	u_s_texture2;
uniform sampler2D	u_s_texture3;
uniform samplerCube u_s_Qtexture0;

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

vec3	v_reflect;
uniform		vec3	u_camera;
vec3	v_toEye;

uniform		float	u_time;
varying		vec2	v_uv;

vec4 finalColor;


vec4 reflection(vec2 uv){

	vec3 normalT	= texture2D(u_s_texture0, uv).xyz;
	mat3 TBN		= mat3(normalize(v_tangent_W),normalize(v_binorm_W),normalize(v_norm_W));
    vec3 v_norm2	= normalize(TBN * (2.0 * normalT - 1.0));
    
																			
	v_reflect		= reflect(normalize(-v_toEye),normalize(v_norm2));//v_norm_W debe ser reemplazado por v_norm2
    vec4	 skyT   = textureCube(u_s_Qtexture0, v_reflect);
    
    skyT.a = 1.0;
	
	return skyT;
}




vec4 refraction(vec2 uv){//bottom
	vec4  rockT			= texture2D(u_s_texture2, uv);
	//rockT = mix(rockT , vec4 (0.0,0.0,1d.,1.0), 0.1);
	
	vec3 v_norm2	= v_norm_W;
	
	vec3 DiffuseTotal	= vec3(0.0,0.0,0.0);
	vec3 diffPosDir;
	vec3 reflectDir;
	float DiffuseIntensity	 = 0.0;
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
	}	
	float u_depthAdjust = 0.1;
	float depthFactor	= clamp(u_depthAdjust * v_uv.x, 0.0, 1.0);
	
	vec4 refractionColor = mix(rockT, vec4(0.0,0.0,1.0,1.0), depthFactor);
	vec4 lightColor		 = vec4(((u_ambCol * u_ambWei + DiffuseTotal * (1.0 - u_ambWei))* rockT.xyz ) , rockT.w);
	return	refractionColor * lightColor;	
}

void main(){
	/**
		TODO: 
		- reflection must handle the uv variable (v_uv + offset) NOT refraction.
		- fresnel is giving problemns when mixing the reflection with refraction.	
	*/


	v_toEye =  (u_camera - v_pos_W);

	vec2  displaT     = texture2D(u_s_texture1, vec2(v_uv.x, v_uv.y + (u_time/2.5))).xy;
	vec2 offsetRefrac = (2.0 * displaT -1.0) * (v_uv.x * 0.05);
	vec2 uv = v_uv + offsetRefrac;
	
	float fresnel = pow((1.0-abs(dot(normalize(v_norm_W),normalize(v_toEye)))),2.0);				
	vec4	mixed = vec4(mix(refraction(uv), reflection(uv), fresnel).xyz,1.0);
	mixed.a = 1.0;
	gl_FragColor  = mixed;
	//gl_FragColor  = refraction(uv);
	//gl_FragColor  = reflection(uv);
}