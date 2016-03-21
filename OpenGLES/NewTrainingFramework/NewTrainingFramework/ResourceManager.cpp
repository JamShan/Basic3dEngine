
#include "stdafx.h" 

#include "ResourceManager.h"
#include "Globals.h"
#include "Texture.h"
#include "Model.h"
#include "Shaders.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>



ResourceManager * ResourceManager::instance = NULL;
ResourceManager * ResourceManager::getInstance(){
	if( instance == NULL)
		instance = new ResourceManager();
	return instance;
}

void  ResourceManager::destroyInstance(){
	SAFE_DELETE(instance);	
}

ResourceManager::ResourceManager():m_noModels(0),m_noTextures(0),m_noShaders(0){
	m_ModelArr		= NULL;
	m_TextureArr	= NULL;
	m_ShaderArr		= NULL;

} 

ResourceManager::~ResourceManager(){
	int i;
	for(i=0;i<m_noModels;i++){
		SAFE_DELETE(m_ModelArr[i]);
	}
	SAFE_DELETE(m_ModelArr);

	for(i=0;i<m_noTextures;i++){
		SAFE_DELETE(m_TextureArr[i]);
	}
	SAFE_DELETE(m_TextureArr);
	
	for(i=0;i<m_noQTextures;i++){
		SAFE_DELETE(m_QTextureArr[i]);
	}
	SAFE_DELETE(m_QTextureArr);

	for(i=0;i<m_noShaders;i++){
		SAFE_DELETE(m_ShaderArr[i]);
	}
	SAFE_DELETE(m_ShaderArr);

} 

void ResourceManager::Init(char * filename){
	
	unsigned int ind = 0, id = 0, aux;
	char param1[256], param2[256];

	memset(param1,'\0',sizeof(param1));
	memset(param2,'\0',sizeof(param2));
		
	FILE *pFile1 = fopen(filename, "r");
	
    if (pFile1 == NULL) {
       printf("Error: no fue posible abrir el archivo %s\n",filename);
	   return;
    }
	printf("\nSuccesfully opened file: %s\n",filename);
	
	fscanf_s( pFile1, "ModelsCount %d\n",&m_noModels);
	printf("ModelsCount %d\n",m_noModels);

	m_ModelArr = new  Model*[m_noModels];

	for ( ; ind < m_noModels ; ind++ ){
		fscanf_s( pFile1, "ID	%d\nFILE \"%s\n",&id , param1, sizeof(param1));
		if( id != ind){
			printf("-WARNING- Not consecutive Model Numbering\n");
			return;
		}
		param1[strlen(param1)-1] = '\0';
		printf("model[%d] = %s; -", id, param1);
		m_ModelArr[ind] = new Model();
		m_ModelArr[ind]->InitModel(id, param1);
		printf("- LOADED\n");
	}

	
	fscanf_s( pFile1, "2DTexturesCount %d\n",&m_noTextures);
	printf("\n2DTexturesCount %d\n",m_noTextures);
	if( m_noTextures){
		(m_TextureArr) = new Texture*[m_noTextures];
		for ( ind = 0 ; ind < m_noTextures ; ind++ ){
			fscanf_s( pFile1, "ID %d\nFILE	\"%s\nTILING %s\n",	&id, param1,sizeof(param1), param2,sizeof(param2));
			if( id != ind){
				printf("-WARNING- Not consecutive Model Numbering\n");
				return;
			}
			if( strcmp("GL_REPEAT",param2) == 0){
				aux = GL_REPEAT;
			}
			else{
				aux = GL_CLAMP_TO_EDGE;
			}

			param1[strlen(param1)-1] = '\0';
			m_TextureArr[ind] = new Texture();
			m_TextureArr[ind]->InitTexture(id, param1, aux);

			printf("2Dtexture[%d] = %s; - LOADED\n", id, param1);
		}
	}


	fscanf_s( pFile1, "CubeTexturesCount %d\n",&m_noQTextures);
	printf("\nCubeTexturesCount %d\n",m_noQTextures);
	if(m_noQTextures){
		(m_QTextureArr) = new Texture*[m_noTextures];
		for ( ind = 0 ; ind < m_noQTextures ; ind++ ){
			fscanf_s( pFile1, "ID %d\nFILE \"%s\nTILING %s\n",	&id, param1,sizeof(param1), param2,sizeof(param2));
			if( id != ind){
				printf("-WARNING- Not consecutive Model Numbering\n");
				return;
			}
			if( strcmp("GL_REPEAT",param2) == 0){
				aux = GL_REPEAT;
			}
			else{
				aux = GL_CLAMP_TO_EDGE;
			}

			param1[strlen(param1)-1] = '\0';
			m_QTextureArr[ind] = new Texture();
			m_QTextureArr[ind]->InitCubeTexture(id, param1, aux);

			printf("CubeTexture[%d] = %s; - LOADED\n", id, param1);
		}
	}



	fscanf_s( pFile1, "ShadersCount %d\n",&m_noShaders);
	printf("\nShadersCount: %d\n",m_noShaders);
	if(m_noShaders){
		(m_ShaderArr) = new Shaders*[m_noShaders];
		for ( ind = 0 ; ind < m_noShaders ; ind++ ){
			fscanf_s( pFile1, "%*s %d	%*s \"%s	%*s \"%s",&id, param1,sizeof(param1), param2,sizeof(param2));
			if( id != ind){
				printf("-WARNING- Not consecutive Model Numbering\n");
				return;
			}

			printf("Shaders[%d] = %s\t%s - ", id, param1, param2);

			param1[strlen(param1)-1] = '\0';
			param2[strlen(param2)-1] = '\0';
			m_ShaderArr[ind] = new Shaders();
			m_ShaderArr[ind] ->Init(param1, param2);
			m_ShaderArr[ind] ->setID(id);

			fscanf_s( pFile1, "%*s %d", &aux);
			m_ShaderArr[ind] ->setStatesAmount(aux);
			printf("\tstatesCount: %d\t",aux);
			while( aux-- ){
				fscanf_s( pFile1, "%*s %s", param1,sizeof(param1));	
				printf("%s, ", param1);
				m_ShaderArr[ind] ->addState(param1);
			}
			printf(" - LOADED\n");
		}
	}


	pFile1		= NULL;
}


unsigned int ResourceManager::getModelsCount(){
	return m_noModels;
}

unsigned int ResourceManager::getTexturesCount(){
	return m_noTextures;
}
unsigned int ResourceManager::getQTexturesCount(){
	return m_noQTextures;
}

unsigned int ResourceManager::getShadersCount(){
	return m_noShaders;
}

Model* ResourceManager::getModelByID(unsigned int i_id){
	if( m_ModelArr )
		if( i_id < m_noModels )
			return m_ModelArr[i_id];
	return NULL;
}


Texture* ResourceManager::getTextureByID(unsigned int i_id){
	if( m_TextureArr )
		if( i_id < m_noTextures )
			return m_TextureArr[i_id];
	return NULL;
	
}

Texture* ResourceManager::getQTextureByID(unsigned i_id){
	if( m_QTextureArr )
		if( i_id < m_noQTextures )
			return m_QTextureArr[i_id];
	return NULL;
	
}


Shaders* ResourceManager::getShaderByID(unsigned int i_id){
	if( m_ShaderArr )
		if( i_id < m_noShaders )
			return m_ShaderArr[i_id];
	return NULL;
	
}

