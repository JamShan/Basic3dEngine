#include "stdafx.h"
#include "Texture.h"
#include "Globals.h"

/*Texture mapping using a TGA image:
 -> CPU code part
*/
Texture::Texture():textureHandle(0), m_ID(0){
}


Texture::~Texture(){
	if(textureHandle){
		glDeleteBuffers(1,&textureHandle);
		textureHandle = NULL;
	}
}
 
void Texture::InitTexture(unsigned ID, const char * file, GLuint tiling){// create the OpenGL ES texture resource and get the handle

	m_ID = ID;
	glGenTextures(1, &textureHandle);

	glBindTexture(GL_TEXTURE_2D, textureHandle);// bind the texture to the 2D texture type

	// create CPU buffer and load it with the image data
	int width, height, bpp;
	char * bufferTGA = LoadTGA(file, &width, &height, &bpp );

 
	// load the image data into OpenGL ES texture resource
	GLint gl_rgb_a = (bpp==24?GL_RGB:GL_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_rgb_a ,width,height,0, gl_rgb_a ,GL_UNSIGNED_BYTE, bufferTGA);

	// free the client memory
	delete [] bufferTGA;

	//set the filters for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// generate the mipmap chain
	glGenerateMipmap(GL_TEXTURE_2D);


	//set the wrapping modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tiling);
}



bool Texture::InitCubeTexture(unsigned ID, const char * path, GLuint tiling){

	m_ID = ID;
	// create the OpenGL ES texture resource and get the handle
	glGenTextures(1, &textureHandle);

	// bind the texture to the 3D texture type
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

	// create CPU buffer and load it with the image data
	int width, height, bpp;
	char * bufferTGA = LoadTGA(path, &width, &height, &bpp );	
	int w = width/4;
	int h = height/3; // w == h 
	int BytesPerTile = w*w*bpp/8;

	char * p[6];
	for(int i=0; i<6; i++)
		p[i] = new char[BytesPerTile];

	ExtractFace(bufferTGA, p[0], width, bpp, 2 * w, h);
	ExtractFace(bufferTGA, p[1], width, bpp, 0, h);
	ExtractFace(bufferTGA, p[2], width, bpp, w, 0);
	ExtractFace(bufferTGA, p[3], width, bpp, w, 2 * h);
	ExtractFace(bufferTGA, p[4], width, bpp, w, h);
	ExtractFace(bufferTGA, p[5], width, bpp, 3 * w, h);

	GLint gl_rgb_a = (bpp==24?GL_RGB:GL_RGBA);
	for(int i=0;i<6;i++){
		// load the image data into OpenGL ES texture resource
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, gl_rgb_a, w, w, 0, gl_rgb_a ,GL_UNSIGNED_BYTE, p[i]);
		//Deleting p
		SAFE_DELETE(p[i]);
	}
	// free the client memory
	SAFE_DELETE_ARR(bufferTGA);

	//set the filters for minification and magnification
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// generate the mipmap chain
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//set the wrapping modes
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tiling);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tiling);
	return true;
}

char* Texture::ExtractFace(char * bufferTGA, char * tmp, int width, int bpp, int offsetX, int offsetY)
{
	int w = width/4;
	int Bpp = bpp/8;
	int startRead = offsetY * width * Bpp + offsetX * Bpp;

	for (int i = 0; i < w; i++){
		memcpy_s(tmp + i * w *Bpp, w*w*Bpp,bufferTGA + startRead,w * Bpp);
		startRead += width * Bpp;
	}
	return tmp;
}