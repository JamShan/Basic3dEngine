

#include "stdafx.h" 
#include "Camera.h"
#include "Globals.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(float i_fov, float i_near,float i_far, float i_speed):
angleX(0.0f),angleY(0.0f),m_rotSpeed(60.0f){
	m_near	= i_near;
	m_far	= i_far;
	m_fov	= i_fov;
	m_speed	= i_speed;
	Init();
}


void Camera::Init(){

	ProjectionM.SetPerspective(m_fov,((float)Globals::screenWidth/Globals::screenHeight),m_near,m_far);
	
	transM.SetIdentity();
	rotX.SetIdentity();
	rotY.SetIdentity();
	
	camera_move.z = 40.0f;
	camera_move.y = 15.0f;
	angleX = -0.5;
}
//trans
void Camera::moveFront(bool front, float deltaTime){
	Vector4 moveL;
    Matrix  worldMatrix;
    Vector4 moveW ;
   
    worldMatrix = getWorldM();
	if (front)
	    moveL = Vector4(0, 0, -deltaTime * m_speed,1);
	else
		moveL = Vector4(0, 0, deltaTime * m_speed,1);
    moveW = moveL * worldMatrix;

    camera_move.x += moveW.x;
    camera_move.y += moveW.y;
    camera_move.z += moveW.z;
}



void Camera::moveUp(bool up, float deltaTime){
	Vector4 moveL;
    Matrix  worldMatrix;
    Vector4 moveW ;
   
    worldMatrix = getWorldM();
	if (up)
	    moveL = Vector4(0, -deltaTime * m_speed, 0, 1);
	else
		moveL = Vector4(0, deltaTime * m_speed, 0, 1);
    moveW = moveL * worldMatrix;

    camera_move.x += moveW.x;
    camera_move.y += moveW.y;
    camera_move.z += moveW.z;
}

void Camera::moveSides(bool left, float deltaTime){
	//movx  += ((float)(left? -1 : 1) * deltaTime * speed);
	Vector4 moveL;
    Matrix  worldMatrix;
    Vector4 moveW ;
   
    worldMatrix = getWorldM();
	if (left)
	    moveL = Vector4( -deltaTime * m_speed, 0, 0,1);
	else
		moveL = Vector4(deltaTime * m_speed, 0, 0,1);
    moveW = moveL * worldMatrix;

    camera_move.x += moveW.x;
    camera_move.y += moveW.y;
    camera_move.z += moveW.z;
}
//rot
void Camera::rotUp(bool up, float deltaTime){
	if(up){
		if( (angleX*180/M_PI) <= 90)
			angleX += ((float)deltaTime * m_rotSpeed * M_PI / 180);
	}else{
		if( (angleX*180/M_PI) >= -90)
			angleX -= ((float)deltaTime * m_rotSpeed * M_PI / 180);
	}
}

void Camera::rotSides(bool left, float deltaTime){
	if(left){
		angleY += ((float)deltaTime * m_rotSpeed * M_PI / 180);
	}else{
		angleY -= ((float)deltaTime * m_rotSpeed * M_PI / 180);	
	}
}


//matrix
Matrix Camera::getViewM(){
	Matrix ViewM;
	
	/* viewMatrix = inverse( worldMatrix);
				  = inverse(rotZ  * rotX * rotY * trans);
				  = inverse(trans) * inverse(rotY) * inverse(rotX) * inverse(rotZ);
	
		inverse(trans)= setTranslation(-camPos.x, -camPos.y, -camPos.z);
		inverse(rotX)= setRotationX(-rotAngle.x);
	*/

	//actualizamos matrix de translacion y las 2 de rotacion
	transM.SetTranslation(-camera_move.x, -camera_move.y, -camera_move.z);

	rotX.SetRotationX(-angleX);	
	rotY.SetRotationY(-angleY);

	//inversa quedaría así
	ViewM = transM * rotY * rotX;
	return ViewM;
}

Matrix Camera::getWorldM(){
	//WorldM = rotX * rotY * transM;
	//return WorldM;

	Matrix    worldMatrix;
    Matrix cam_rotationXMatrix;
    Matrix cam_rotationYMatrix;
    Matrix cam_translationMatrix;   

    cam_rotationXMatrix.SetIdentity();
    cam_rotationYMatrix.SetIdentity();
    cam_translationMatrix.SetIdentity();

	cam_rotationXMatrix.SetRotationX(angleX);
	cam_rotationYMatrix.SetRotationY(angleY);

    worldMatrix = cam_rotationXMatrix * cam_rotationYMatrix * cam_translationMatrix;

    return worldMatrix;

}

Matrix Camera::getProjectM(){
	return ProjectionM;
}

Vector3 Camera::getPosAsArray(){
	return camera_move;
}