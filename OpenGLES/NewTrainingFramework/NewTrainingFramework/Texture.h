#ifndef __TEXTURE__
#define __TEXTURE__

#include "../Utilities/Math.h"
#include "../Utilities/TGA.h"
#include "../Utilities/esUtil.h"
#include "Vertex.h"


class Texture
{

public:
//ONSTRUCTOR
	Texture();
	~Texture();

 //Functions
    void InitTexture(unsigned ID, const char* file, GLuint tiling);                        
	bool InitCubeTexture(unsigned ID, const char* path, GLuint tiling);
	char* ExtractFace(char* bufferTGA, char* tmp, int width, int bpp, int offsetX, int offsetY);
	void Draw();


//MEMBERS
	GLuint textureHandle;
	unsigned m_ID;

};
#endif