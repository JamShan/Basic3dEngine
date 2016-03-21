#include "stdafx.h"
#include "Globals.h"
#include "Object.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"
#include "Model.h"
#include "ResourceManager.h"
#include "SceneManager.h"


Object::Object(GLuint objectID): 
m_ID(0),				m_2DTexturesCount(0),	m_2Dcounter(0),
WMchanged(false),		m_CubeTexturesCount(0),	m_Cubecounter(0),
m_LightsCount(0),		m_Lightscounter(0)
{
	m_ID = objectID;
	m_Model			= NULL;
	m_Textures		= NULL;
	m_CubeTextures	= NULL;
	m_Shader		= NULL;
	m_Lights		= NULL;
	WM.SetIdentity();
}

Object::~Object(){
	m_Model		= NULL;
	m_Shader	= NULL;	
	SAFE_DELETE_ARR(m_Textures);	
	SAFE_DELETE_ARR(m_CubeTextures);
	SAFE_DELETE_ARR(m_Lights);
}


void Object::setModelID(unsigned int id){
	m_Model = ResourceMan->getModelByID(id);
}


void Object::setLightsCount(unsigned int lightsCount){
	if( lightsCount ){
		m_LightsCount = lightsCount;
		m_Lights = new Light*[lightsCount];
	}
	
}

void Object::addLightID(unsigned int id){
	if( m_Lightscounter < m_LightsCount ) 
		m_Lights[m_Lightscounter++] = SceneMan->getLightByID(id);
}

void Object::set2DTextureCount(unsigned int texturesCount){
	if( texturesCount ){
		m_2DTexturesCount = texturesCount;
		m_Textures = new Texture*[texturesCount];
	}
	
}

void Object::add2DTextureID(unsigned int id){
	if( m_2Dcounter < m_2DTexturesCount ) 
		m_Textures[m_2Dcounter++] = ResourceMan->getTextureByID(id);
}

void Object::setCubeTextureCount(unsigned int texturesCount){
	if(texturesCount){
		m_CubeTexturesCount = texturesCount;
		m_CubeTextures = new Texture*[texturesCount];
	}
	
}

void Object::addCubeTextureID(unsigned int id){
	if( m_Cubecounter < m_CubeTexturesCount ) 
		m_CubeTextures[m_Cubecounter++] = ResourceMan->getQTextureByID(id);
}

void Object::setShaderID(unsigned int id){
	m_Shader = ResourceMan->getShaderByID(id);
}

void Object::setPos(float x, float y, float z){
	pos.x=x;	pos.y=y;	pos.z=z;
	WMchanged = true;
}

void Object::setRot(float x, float y, float z){
	rot.x=x;	rot.y=y;	rot.z=z;
	WMchanged = true;
}

void Object::setScale(float x, float y, float z){
	scale.x=x;	scale.y=y;	scale.z=z;
	WMchanged = true;
}

void Object::Draw(){

	unsigned short textureUnit;
	for(textureUnit = 0;textureUnit<m_Shader->statesCount;textureUnit++){
		glEnable(m_Shader->states[textureUnit]);
		if(m_Shader->states[textureUnit] == GL_BLEND)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glUseProgram(m_Shader->program);
	glBindBuffer(GL_ARRAY_BUFFER,			m_Model->m_hVertexBuffer);
		
	if(m_Shader->positionAttribute != -1){
		glEnableVertexAttribArray(m_Shader->positionAttribute);
		glVertexAttribPointer(m_Shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	//Task1 - Color
	if(m_Shader->colorAttribute != -1){
		glEnableVertexAttribArray(m_Shader->colorAttribute);
		//							atribute		#elements SizeElem			SizeStructure      offset
		glVertexAttribPointer(m_Shader->colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
	}
	//Task5 - Normal
	if(m_Shader->attributeNormal != -1){
		glEnableVertexAttribArray(m_Shader->attributeNormal);
		glVertexAttribPointer(m_Shader->attributeNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	}
	//Task5 - Binormal
	if(m_Shader->attributeBinormal != -1){
		glEnableVertexAttribArray(m_Shader->attributeBinormal);
		glVertexAttribPointer(m_Shader->attributeBinormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(10 * sizeof(float)));
	}
	//Task5 - Tangent
	if(m_Shader->attributeTangente != -1){
		glEnableVertexAttribArray(m_Shader->attributeTangente);
		glVertexAttribPointer(m_Shader->attributeTangente, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(13 * sizeof(float)));
	}

	//Task5 - UV ->load texture
	if ( m_Shader->attributeUV != -1){
		glEnableVertexAttribArray(m_Shader->attributeUV);
		glVertexAttribPointer(m_Shader->attributeUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(16*sizeof(float)));
	}

	//TASK2 & 3 & 4 - VIEW MATRIX
	if(m_Shader->uniformVWPmatrix != -1){
		Matrix auxMat =	updateWM() *  SceneMan->camera->getViewM()	* SceneMan->camera->getProjectM();
		//enviamos a shader
		glUniformMatrix4fv(m_Shader->uniformVWPmatrix, 1, GL_FALSE, &auxMat.m[0][0]);
	}

	//TASK7 - WorldMatrix
	if(m_Shader->uniformWmatrix != -1){
		//glUniformMatrix4fv(m_Shader->uniformWmatrix, 1, GL_FALSE, (GLfloat*)updateWM().m);
		glUniformMatrix4fv(m_Shader->uniformWmatrix, 1, GL_FALSE, &updateWM().m[0][0]);
	}

	//TASK7 - Camera Position
	if(m_Shader->uniformCamera != -1){
		//enviamos a shader
		glUniform3fv(m_Shader->uniformCamera, 1, &(SceneMan->camera->getPosAsArray().x));
	}
	
	//TASK7 - Fog Properties
	if(m_Shader->uniformFogColor != -1){
		glUniform4fv(m_Shader->uniformFogColor,1,((const GLfloat *)SceneMan->fog->getColor()));
	}	

	if(m_Shader->uniformFogStart != -1){
		glUniform1f(m_Shader->uniformFogStart,((GLfloat)SceneMan->fog->Start));
	}

	if(m_Shader->uniformFogRange != -1){
		glUniform1f(m_Shader->uniformFogRange,((GLfloat)SceneMan->fog->Range));
	}
	
	//task9 Fire
	if(m_Shader->uniformTime != -1){
		glUniform1f(m_Shader->uniformTime,((GLfloat)SceneMan->updateTime));		
	}


	//task10 Light
	if(m_Shader->uniformAmbCol != -1){
		//glUniform4fv(m_Shader->uniformAmbCol,1,((const GLfloat *)SceneMan->fog->getColor()));
		glUniform3fv(m_Shader->uniformAmbCol, 1, &(SceneMan->Amb_Color.x) );
	}

	if(m_Shader->uniformAmbWei != -1){
		glUniform1f(m_Shader->uniformAmbWei,((GLfloat)SceneMan->Amb_W));		
	}

	if(m_Shader->uniformNumLig != -1){
		//glUniform4fv(m_Shader->uniformAmbCol,1,((const GLfloat *)SceneMan->fog->getColor()));
		glUniform1i(m_Shader->uniformNumLig, m_Lightscounter );
	}

	if(m_Lightscounter){
		if(m_Shader->uniformLigCol != -1){
			//if this doesn't work, get this object's lights colors into the same array
			glUniform3fv(m_Shader->uniformLigCol, m_Lightscounter, &m_Lights[0]->Color.x);
		}

		if(m_Shader->uniformLigDir != -1){
			glUniform3fv(m_Shader->uniformLigDir, m_Lightscounter, &m_Lights[0]->Pos.x);
		}

		if(m_Shader->uniformLigTyp != -1){
			glUniform1iv(m_Shader->uniformLigTyp, m_Lightscounter, &m_Lights[0]->type);
		}
	}

	textureUnit = 0;
	//m_CubeTexturesCount
	if(m_CubeTexturesCount && m_Shader->uniformQtexture!=-1){//task 8
		glEnable(GL_TEXTURE_CUBE_MAP);
		//glDisable(GL_TEXTURE_2D);
		// make active a texture unit
		glActiveTexture(GL_TEXTURE0+textureUnit);
		// bind the texture to the currently active texture unit
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_CubeTextures[textureUnit]->textureHandle);

		glUniform1i( m_Shader->uniformQtexture, textureUnit);
		textureUnit++;
	}
	
	int textures = 0;
	for( ; textures < m_2Dcounter ; textures++, textureUnit++){
		if(m_Shader->txtr[textures]!=-1){
			glEnable(GL_TEXTURE_2D);
			//glDisable(GL_TEXTURE_CUBE_MAP);
			// make active a texture unit
			glActiveTexture(GL_TEXTURE0+textureUnit);
			// bind the texture to the currently active texture unit
			glBindTexture(GL_TEXTURE_2D, m_Textures[textures]->textureHandle);
			// set the uniform sampler
			glUniform1i(m_Shader->txtr[textures], textureUnit);
		}
	}

			
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	m_Model->m_hIndexBuffer);
	glDrawElements(GL_TRIANGLES, m_Model->m_noIndices, GL_UNSIGNED_INT, (void * )0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(textureUnit = 0;textureUnit<m_Shader->statesCount;textureUnit++){
		//glDisable(m_Shader->states[textureUnit]);
	}
}

void Object::Update(){
	
}

Matrix Object::updateWM(){
	if( WMchanged ){
		Matrix rotXMat, rotYMat, rotZMat, scaleMat, transMat;

		
		//Init Matrices
		rotXMat.SetIdentity();		rotYMat.SetIdentity();		rotZMat.SetIdentity(); 
		scaleMat.SetIdentity(); 	transMat.SetIdentity();
		

		rotXMat.SetRotationX(rot.x);
		rotYMat.SetRotationY(rot.y);
		rotZMat.SetRotationZ(rot.z);

		scaleMat.SetScale(scale);

		transMat.SetTranslation(pos);
	 
		// scala, rotacion, translacion
		WM = scaleMat * rotXMat * rotYMat * rotZMat * transMat;
		WMchanged = false;
	}
	return WM;
}