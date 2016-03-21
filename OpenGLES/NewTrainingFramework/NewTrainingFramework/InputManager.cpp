
#include "stdafx.h" 
#include "InputManager.h"
#include "Camera.h"
#include "Globals.h"
#include <math.h>

InputManager * InputManager::instance = NULL;

InputManager * InputManager::getInstance(){
	if( instance == NULL)
		instance = new InputManager();
	return instance;
}

void  InputManager::destroyInstance(){
	SAFE_DELETE(instance);
}

void InputManager::transformKeyVal(int &KeyVal){
	//printf("\t key received: %c with val:%d,",KeyVal,KeyVal);
	if (KeyVal < 41) //this is for arrows
		KeyVal += 24;

	KeyVal-=60;//this is to get smaller numbers for bitwise operations
	//printf(" transformedTo= %d\n",KeyVal);
}

void InputManager::pressKey(int KeyVal){
	transformKeyVal(KeyVal);
	keyPressed |= 1<<KeyVal;
}

void InputManager::releaseKey(int KeyVal){
	transformKeyVal(KeyVal);
	keyPressed &=~ (1<<KeyVal);
}

bool InputManager::isKeyPressedTrans(int KeyVal){
	transformKeyVal(KeyVal);
	return isKeyPressed(KeyVal);
}


bool InputManager::isKeyPressed(int KeyVal){
	return ((keyPressed | 1<<KeyVal) == keyPressed);
	//return (keyPressed & (1<<KeyVal) != 0);
}

void InputManager::handleKeys(float deltaTime , Camera *camera ){

	if(isKeyPressed(Globals::_KEY_E_T)){
		printf(" KEY E PRESSED\n" );
		camera->moveUp(false, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_Q_T)){
		printf(" KEY Q PRESSED\n" );
		camera->moveUp(true, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_A_T)){
		printf(" KEY A PRESSED\n" );
		camera->moveSides(true, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_S_T)){
		printf(" KEY S PRESSED\n" );
		camera->moveFront(false, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_D_T)){
		printf(" KEY D PRESSED\n" );
		camera->moveSides(false, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_W_T)){
		printf(" KEY W PRESSED\n" );
		camera->moveFront(true, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_LEFT_T)){
		printf(" KEY LEFT PRESSED\n" );
		camera->rotSides(true, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_UP_T)){
		printf(" KEY UP PRESSED\n" );
		camera->rotUp(true, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_RIGHT_T)){
		printf(" KEY RIGHT PRESSED\n" );
		camera->rotSides(false, deltaTime);
	}
	if(isKeyPressed(Globals::_KEY_DOWN_T)){
		printf(" KEY DOWN PRESSED\n" );
		camera->rotUp(false, deltaTime);
	}
}