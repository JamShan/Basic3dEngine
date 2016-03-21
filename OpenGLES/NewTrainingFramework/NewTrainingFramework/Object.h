
#ifndef _OBJECT_
#define _OBJECT_

#include "..\Utilities\Math.h"
#include "Globals.h"

class Texture;
class Model;
class Shaders;
class Light;
class Math;


class Object{

public:
	Object(GLuint objectID);
	~Object();

	void setModelID(unsigned int id);
	void setLightsCount(unsigned int texturesCount);
	void addLightID(unsigned int id);
	void set2DTextureCount(unsigned int texturesCount);
	void add2DTextureID(unsigned int id);
	void setCubeTextureCount(unsigned int texturesCount);
	void addCubeTextureID(unsigned int id);
	void setShaderID(unsigned int id);	
	void setPos(float x, float y, float z);
	void setRot(float x, float y, float z);
	void setScale(float x, float y, float z);
	void Draw();
	void Update();
	Matrix updateWM();

private:
	GLuint	m_ID;
	Model	*m_Model;
	Texture **m_Textures;
	Texture **m_CubeTextures;
	Shaders	*m_Shader;
	Light	**m_Lights;
	Vector3 pos, rot, scale;
	bool	WMchanged;

	Matrix WM;

	unsigned int m_2DTexturesCount,  m_2Dcounter, m_CubeTexturesCount,  m_Cubecounter, m_LightsCount, m_Lightscounter;

};

#endif

