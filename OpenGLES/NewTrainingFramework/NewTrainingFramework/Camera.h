#ifndef __CAMERA__
#define __CAMERA__

#include "../Utilities/Math.h"

class Camera{

	Vector4 moveL, moveW;
	Vector3 camera_move;
	Matrix ProjectionM, transM, rotX, rotY;
	float angleX, angleY, m_speed, m_near, m_far, m_fov, m_rotSpeed;


public:
	Camera(float i_fov, float i_near,float i_far, float i_speed);
	~Camera(){}

	void Init(); 

	void moveUp(bool up, float deltaTime);
	void moveFront(bool front, float deltaTime);
	void moveSides(bool left, float deltaTime);
	void move();

	void rotUp(bool up, float deltaTime);
	void rotSides(bool up, float deltaTime);
	void rotate();

	Matrix getViewM();
	Matrix getWorldM();
	Matrix getProjectM();
	Vector3 getPosAsArray();

};

#endif