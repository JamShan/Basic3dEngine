#pragma once

#define SAFE_DELETE(x)		if(x){delete x;		x=NULL;}
#define SAFE_DELETE_ARR(x)	if(x){delete[] x;	x=NULL;}

class Globals
{
public:
	static const int screenWidth	= 960,
					 screenHeight	= 720;

	static const int _KEY_LEFT		= 37, _KEY_LEFT_T		= 1, 
					 _KEY_UP		= 38, _KEY_UP_T			= 2,
					 _KEY_RIGHT		= 39, _KEY_RIGHT_T		= 3,
					 _KEY_DOWN		= 40, _KEY_DOWN_T		= 4,
					 _KEY_A			= 65, _KEY_A_T			= 5,
					 _KEY_D			= 68, _KEY_D_T			= 8,
					 _KEY_E			= 69, _KEY_E_T			= 9,
					 _KEY_Q			= 81, _KEY_Q_T			= 21,
					 _KEY_S			= 83, _KEY_S_T			= 23,
					 _KEY_W			= 87, _KEY_W_T			= 27;


};