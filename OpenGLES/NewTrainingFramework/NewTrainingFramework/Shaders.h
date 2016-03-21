#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint colorAttribute;//TASK1
	GLint uniformVWPmatrix;//TASK2
	//GLint uniformLocation;//TASK5
	GLint attributeUV;//TASK5
	GLint attributeNormal;//TASK5
	GLint attributeBinormal;//TASK5
	GLint attributeTangente;//TASK5
	

	//MultiTexturing
	GLint uvTilingFactor;//TASK7
	GLint txtr[4];

	//FOG
	GLint uniformFogColor;//TASK7
	GLint uniformFogStart;
	GLint uniformFogRange;
	GLint uniformCamera;
	GLint uniformWmatrix;

	//CubeTexture
	GLint uniformQtexture;//TASK8

	//Reflection
	//GLint varyingToEye;//TASK8
	//GLint varyingReflectDir;

	//Fire
	GLint uniformTime;//TASK9

	//Light
	GLint uniformNumLig;
	GLint uniformAmbCol;//Task10
	GLint uniformAmbWei;
	GLint uniformLigCol;
	GLint uniformLigDir;
	GLint uniformLigTyp;
	
	

	unsigned int m_ID;

	int Init(char* fileVertexShader, char* fileFragmentShader);

	void setID(unsigned int i_ID);
	void addState(char* state);
	void setStatesAmount(int amount);
	int statesAmount, statesCount;
	int* states;

	Shaders():statesAmount(0), statesCount(0){};
	~Shaders();

	
};