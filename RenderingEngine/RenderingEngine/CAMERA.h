#ifndef __CAMERA_H__
#define __CAMERA_H__
#define KEY_W 0
#define KEY_D 1
#define KEY_S 2
#define KEY_A 3
#define KEY_Q 4
#define KEY_E 5


#define MOUSE_RIGHT_BUTTON 0
#define MOUSE_LEFT_BUTTON 1

#include <d3dx9.h>

class CAMERA
{
	struct Speed
	{
		float s;
		float th, pi;
	};
	void CalculateConst();

public:
	void Initialize();
	void Move(unsigned int);

	void rotation(int, int, int);
	void SetPosition();


	struct Speed speed;
	float angle[2]; // in Radian
	float constant[2][2];
	float fov;		// in Degree
	float near_;
	float far_;
	float length;
	
	D3DXMATRIX V;
	D3DXMATRIX proj;

	D3DXVECTOR3 position;
	D3DXVECTOR3 target;
	D3DXVECTOR3 up;
	D3DXVECTOR3 lookx;
	D3DXVECTOR3 looky;
	D3DXVECTOR3 lookz;


	CAMERA();
	~CAMERA();
};

#endif