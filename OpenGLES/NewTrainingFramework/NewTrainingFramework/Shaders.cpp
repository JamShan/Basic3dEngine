#include "stdafx.h"
#include "Shaders.h"
#include "Globals.h"

Shaders::~Shaders()
{
	if( statesCount){
		SAFE_DELETE(states);
	}
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);
	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute	= glGetAttribLocation(program, "a_posL");
	colorAttribute		= glGetAttribLocation(program, "a_color");//TASK1
	uniformVWPmatrix	= glGetUniformLocation(program, "u_VWPmatrix");//TASK2

	//Model properties
	attributeUV			= glGetAttribLocation(program, "a_uv");
	attributeNormal		= glGetAttribLocation(program, "a_norm");
	attributeBinormal	= glGetAttribLocation(program, "a_binorm");
	attributeTangente	= glGetAttribLocation(program, "a_tangent");

	//Terrain MultiTexturing
	txtr[0]			= glGetUniformLocation(program, "u_s_texture0");
	txtr[1]			= glGetUniformLocation(program, "u_s_texture1");
	txtr[2]			= glGetUniformLocation(program, "u_s_texture2");
	txtr[3]			= glGetUniformLocation(program, "u_s_texture3");

	//Terrain FOG
	uniformFogColor	= glGetUniformLocation(program, "u_fogColor");
	uniformFogStart	= glGetUniformLocation(program, "u_fogStart");
	uniformFogRange	= glGetUniformLocation(program, "u_fogRange");
	uniformCamera	= glGetUniformLocation(program, "u_camera");
	uniformWmatrix	= glGetUniformLocation(program, "u_Wmatrix");

	//CubeTexture
	uniformQtexture	= glGetUniformLocation(program, "u_s_Qtexture0");

	//Reflection
	//varyingToEye		= glGetUniformLocation(program, "v_toEye");
	//varyingReflectDir	= glGetUniformLocation(program, "v_reflectDir");

	//Fire
	uniformTime		= glGetUniformLocation(program, "u_time");

	//Light
	uniformNumLig	= glGetUniformLocation(program, "u_noLights");
	uniformAmbCol	= glGetUniformLocation(program, "u_ambCol");
	uniformAmbWei	= glGetUniformLocation(program, "u_ambWei");
	uniformLigDir	= glGetUniformLocation(program, "u_ligDirAr");
	uniformLigCol	= glGetUniformLocation(program, "u_ligColAr");
	uniformLigTyp	= glGetUniformLocation(program, "u_ligTypAr");

	m_ID = -1;
	return 0;
}

void Shaders::setID(unsigned int i_ID){
	m_ID = i_ID;
}


void Shaders::setStatesAmount(int amount){
	if(amount){
		statesAmount=amount;
		states = new int[amount];
	}
}

void Shaders::addState(char* state){
	
	if (statesAmount < statesCount+1)
		return;

	if( !strcmp(state,"DEPTH_TEST") ){
		states[statesCount++]=GL_DEPTH_TEST;

	}else if( !strcmp(state,"CULLING") ){
		states[statesCount++]=GL_CULL_FACE;

	}else if( !strcmp(state,"BLEND") ){
		states[statesCount++]=GL_BLEND;

	}else if( !strcmp(state,"SCISSOR_TEST") ){
		states[statesCount++]=GL_SCISSOR_TEST;

	}else if( !strcmp(state,"STENCIL_TEST") ){
		states[statesCount++]=GL_STENCIL_TEST;
	}
}