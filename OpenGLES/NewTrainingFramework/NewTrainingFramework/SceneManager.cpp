
#include "stdafx.h" 
#include "Globals.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Object.h"
#include "Model.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdlib.h>

SceneManager * SceneManager::instance = NULL;

SceneManager * SceneManager::getInstance(){
	if( instance == NULL)
		instance = new SceneManager();
	return instance;
}

void  SceneManager::destroyInstance(){
	SAFE_DELETE(instance);
}


SceneManager::SceneManager() : m_noObjects(0), updateTime(0){
	camera		= NULL; 
	m_ObjectArr	= NULL;
} 

SceneManager::~SceneManager(){
	int i;
	for(i=0;i<m_noObjects;i++){
		SAFE_DELETE(m_ObjectArr[i]);
	}
	SAFE_DELETE(m_ObjectArr);

	for(i=0;i<m_noLights;i++){
		SAFE_DELETE(m_LightArr[i]);
	}
	SAFE_DELETE(m_LightArr);
	
	SAFE_DELETE(fog);

	SAFE_DELETE(camera);
} 

void SceneManager::Init(char * filename){
	
	FILE *pFile1 = fopen(filename, "r");
	unsigned int ind = 0, id = 0, modelID, textureID, shaderID, LightID, aux;
	float f1, f2, f3, f4;
	char param1[256], param2[256];

	memset(param1,'\0',sizeof(param1));
	memset(param2,'\0',sizeof(param2));

	if (pFile1 == NULL) {
       printf("Error: no fue posible abrir el archivo %s\n",filename);
	   return;
    }
	printf("\nSuccesfully opened file: %s\n",filename);


	//#LIGHTS
	printf("\n#Lights");
	fscanf(pFile1,"#LIGHTS\n ");
	fscanf(pFile1,"AmbientColor %f, %f, %f\n", &Amb_Color.x, &Amb_Color.y, &Amb_Color.z);
	printf("- AC %.2f %.2f %.2f %.2f", Amb_Color.x, Amb_Color.y, Amb_Color.z);
	fscanf(pFile1,"AmbientWeight %f\n", &Amb_W);
	fscanf(pFile1,"LightsCount %d\n", &m_noLights);
	printf(" - AW%.2f \n", Amb_W);	
	m_LightArr = new Light*[m_noLights];
	for ( ind = 0 ; ind < m_noLights ; ind++ ){
		fscanf(pFile1,"ID %d\n", &LightID);
		m_LightArr[ind] = new Light();

		fscanf(pFile1,"POS/DIR %f, %f, %f\n", &m_LightArr[ind]->Pos.x, &m_LightArr[ind]->Pos.y, &m_LightArr[ind]->Pos.z);
		printf("Light[%d] -  Pos %.2f %.2f %.2f - ", LightID, m_LightArr[ind]->Pos.x, m_LightArr[ind]->Pos.y, m_LightArr[ind]->Pos.z);	

		fscanf(pFile1,"TYPE %s\n", param1, sizeof(param1));
		fscanf(pFile1,"COLOR %f, %f, %f\n", &m_LightArr[ind]->Color.x, &m_LightArr[ind]->Color.y, &m_LightArr[ind]->Color.z);
		printf("T %s - Col %.2f %.2f %.2f %.2f - ",param1, m_LightArr[ind]->Color.x, m_LightArr[ind]->Color.y, m_LightArr[ind]->Color.z);	

		if( !strcmp(param1,"POINT") ){
			m_LightArr[ind]->type=Light::POINT;
			fscanf(pFile1,"MOVING %s\n", param1, sizeof(param1));
			m_LightArr[ind]->moving=Light::CIRCLE;
			fscanf(pFile1,"RADIUS %f\nSPEED %f\n", &m_LightArr[ind]->radius, &m_LightArr[ind]->speed);
			printf(" m %s - R %.2f - S %.2f",param1, m_LightArr[ind]->radius, m_LightArr[ind]->speed);
		} else {
			m_LightArr[ind]->type=Light::DIRECTIONAL;
		}
		printf("- LOADED\n");
	}
	

	fscanf_s( pFile1, "#Objects\nObjectsCount %d\n",&m_noObjects);
	printf("\nObjectsCount: %d\n",m_noObjects);

	m_ObjectArr = new  Object*[m_noObjects];

	for (ind=0 ; ind < m_noObjects ; ind++ ){
		fscanf( pFile1, "ID %d\n",&id);
		
		//Model
		fscanf( pFile1," MODEL_ID %d\n", &modelID);
		printf("Object[%d] = M:%d", id,  modelID);
		m_ObjectArr[ind] = new Object(id);
		m_ObjectArr[ind]->setModelID(modelID);

		
		
		//2DTexutes
		fscanf( pFile1, " 2DTexturesCount %d\n",&aux);
		m_ObjectArr[ind]->set2DTextureCount(aux);
		while( aux-- ){
			fscanf_s(pFile1,"TEXTURE_ID %d\n",&textureID);
			printf(" 2T:%d", textureID);
			m_ObjectArr[ind]->add2DTextureID(textureID);
		}
		
		//CubeTextures
		fscanf_s(pFile1, "CubeTexturesCount %d\n",&aux);
		m_ObjectArr[ind]->setCubeTextureCount(aux);
		while( aux-- ){
			fscanf_s(pFile1,"TEXTURE_ID %d\n",&textureID);
			printf(" cT:%d", textureID);
			m_ObjectArr[ind]->addCubeTextureID(textureID);
		}

		//Shader
		fscanf_s(pFile1, "SHADER_ID %d\n", &shaderID);
		printf(" S:%d", shaderID);
		m_ObjectArr[ind]->setShaderID(shaderID);

		//LightsCount TODO
		fscanf_s(pFile1, "LightsCount %d\n",&aux);
		m_ObjectArr[ind]->setLightsCount(aux);
		while( aux-- ){
			fscanf_s(pFile1,"LIGHT_ID %d\n",&LightID);
			m_ObjectArr[ind]->addLightID(LightID);
			printf(" L:%d", LightID);
		}

		//pos
		fscanf_s(pFile1, "POS %f, %f, %f\n", &f1, &f2, &f3);
		printf("\tPOS:%.2f, %.2f, %.2f", f1,f2,f3);
		m_ObjectArr[ind]->setPos(f1,f2,f3);

		//rot
		fscanf_s(pFile1, "ROTATION %f, %f, %f\n", &f1, &f2, &f3);
		printf("\tROT:%.2f, %.2f, %.2f", f1,f2,f3);
		m_ObjectArr[ind]->setRot(f1,f2,f3);

		//scale
		fscanf_s(pFile1, "SCALE %f, %f, %f\n", &f1, &f2, &f3);
		printf("\tSCALE:%.2f, %.2f, %.2f", f1,f2,f3);
		m_ObjectArr[ind]->setScale(f1,f2,f3);

		printf("- LOADED\n");
	}

	//#CAMERA
	fscanf(pFile1,"\n#CAMERA\n");
	fscanf(pFile1,"NEAR %f\n",&f1);//near
	fscanf(pFile1,"FAR %f\n",&f2);//far
	fscanf(pFile1,"FOV %f\n",&f3);//fov
	fscanf(pFile1,"SPEED %f\n",&f4);//speed
	camera	= new Camera(f3, f1, f2, f4);
	printf("\n\n#Camera - %.2f %.2f %.2f %.2f - LOADED\n", f3, f1, f2, f4);

	//#FOG
	fscanf(pFile1,"\n#FOG\n");
	fscanf(pFile1,"FOG_COLOR %f, %f, %f\n",&f1, &f2, &f3);//color
	fog = new Fog(f1, f2, f3);//instance
	printf("#Fog - C:%.2f, %.2f, %.2f - ",f1, f2, f3);
	fscanf(pFile1,"FOG_START %f\n",&f2);//start
	fog->setStart(f2);
	fscanf(pFile1,"FOG_RANGE %f\n",&f3);//range
	fog->setRange(f3);
	printf("%.2f %.2f - LOADED\n", fog->Start, fog->Range);

	printf("\nAll Objects are successfully LOADED\n\n");
	pFile1		= NULL;	
}


unsigned int SceneManager::getObjectsCount(){
	return m_noObjects;
}


Object* SceneManager::getObjectByID(unsigned int i_id){
	if( m_ObjectArr )
		if( i_id < m_noObjects )
			return m_ObjectArr[i_id];
	return NULL;
}

void SceneManager::Draw(){
	int i =0;//m_noObjects
	for ( ;i<m_noObjects;i++){
		m_ObjectArr[i]->Draw();
	}
}

void SceneManager::Update( float deltaTime ){
	updateTime += deltaTime;

	for(int x = 0;x< m_noLights;x++){
		//m_ObjectArr[x]->setPos(m_LightArr[x]->Pos.x, m_LightArr[x]->Pos.y, m_LightArr[x]->Pos.z);	
		
		if(m_LightArr[x]->type == Light::POINT && m_LightArr[x]->moving == Light::CIRCLE){
			m_LightArr[x]->angle += (m_LightArr[x]->speed * M_PI/180 );
			float newX = m_LightArr[x]->Pos.x - sin(m_LightArr[x]->angle)*m_LightArr[x]->radius;
			float newZ = m_LightArr[x]->Pos.z - cos(m_LightArr[x]->angle)*m_LightArr[x]->radius;
			m_LightArr[x]->Pos = Vector3(newX, m_LightArr[x]->Pos.y, newZ);	
			//m_ObjectArr[x]->setPos(newX, m_LightArr[x]->Pos.y, newZ);	
		}
		
	}
}




/////////////////////////////////////////////////////////////////////////
Light* SceneManager::getLightByID(unsigned int i_id){
	if( m_LightArr && i_id < m_noLights )
		return m_LightArr[i_id];
	return NULL;
}
/////////////////////////////////////////////////////////////////////////