// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Object.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <conio.h>
#include <stdio.h>

//http://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
#define _USE_MATH_DEFINES
#include <math.h>

GLuint		vboId;

int i = 0;

int Init ( ESContext *esContext ){
	glClearColor ( 0.0f, 0.0f, 0.5f, 0.0f );

	ResourceMan	->Init("RM.txt");
	SceneMan	->Init("SM.txt");
	return 0;
}

void Draw ( ESContext *esContext ){
	glClear(GL_COLOR_BUFFER_BIT);
	SceneMan->Draw();
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	SceneMan->Update(deltaTime);
	InputMan->handleKeys(deltaTime, SceneMan->camera);
}
 
void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if( bIsPressed){
		//printf("key Pressed = '%c', val=  %d\n",key, key);
		InputManager::getInstance()->pressKey(key);
	}
	else{
		//printf("key Released= '%c', val=  %d\n",key, key);
		InputManager::getInstance()->releaseKey(key);
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);	

	InputManager::destroyInstance();

	SceneManager::destroyInstance();

	ResourceManager::destroyInstance();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "OpenGLES 2.0 Engine - MultiTexturing", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}