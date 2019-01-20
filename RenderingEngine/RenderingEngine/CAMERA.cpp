#include "CAMERA.h"
#include "GAMESYSTEM.h"

CAMERA::CAMERA()
{
}

CAMERA::~CAMERA()
{
}
void CAMERA::Initialize()
{
	length = 1.0f; // distance between camera position and target

	speed.s = 0.01f;
	speed.pi = speed.th = 0.001f;

	near_ = 1.0f; far_ = 1000.0f;

	fov = 1.047183f;	// 60 degree in radian
	angle[0] = 1.5707745f; // 90 degree in radian
	angle[1] = 0.0f;

	position = { 0.0f, 0.0f, -5.0f };
	up = { 0.0f, 1.0f, 0.0f };

	CalculateConst();
	SetPosition();
}
void CAMERA::Move(unsigned int _data)
{
	// check whether camera is contect with other objects
	if (_data == KEY_W)
	{
		position = position + lookx * speed.s;
		target = position + lookx * length;
	}
	else if (_data == KEY_S)
	{
		position = position - lookx * speed.s;
		target = position + lookx * length;
	}
	else if (_data == KEY_A)
	{
		position = position + lookz * speed.s;
		target = position + lookx * length;
	}
	else if (_data == KEY_D)
	{
		position = position - lookz * speed.s;
		target = position + lookx * length;
	}
	else if (_data == KEY_Q) // 위로
	{
		position = position + looky * speed.s;
		target = position + lookx * length;
	}
	else if (_data == KEY_E) // 아래로
	{
		position = position - looky * speed.s;
		target = position + lookx * length;
	}
	else
		return;
	D3DXMatrixLookAtLH(&gSystem.viewmatrix, &position, &target, &up);
	gSystem.shader->SetMatrix("gViewMatrix", &gSystem.viewmatrix);
}
void CAMERA::SetPosition()
{
	target = position + lookx * length;
	D3DXMatrixLookAtLH(&gSystem.viewmatrix, &position, &target, &up);
	D3DXMatrixPerspectiveFovLH(&gSystem.projmatrix, fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, near_, far_);
	gSystem.shader->SetMatrix("gViewMatrix", &gSystem.viewmatrix);
	gSystem.shader->SetMatrix("gProjectionMatrix", &gSystem.projmatrix);
}
void CAMERA::rotation(int _moveX, int _moveY)
{
	if (gSystem.mouse.IsMouseDown(MOUSE_LEFT) && (_moveX != 0 || _moveY != 0))
	{
		if (_moveX != 0)
		{
			angle[0] += speed.th * _moveX;
		}
		if (_moveY != 0)
		{
			if (constant[1][1] >= 0)
				up.y = 1;
			else
				up.y = -1;

			angle[1] += speed.pi * _moveY;
		}
		CalculateConst();
		SetPosition();
	}
}
void CAMERA::CalculateConst()
{
	constant[0][0] = (float)sin(angle[0]);
	constant[0][1] = (float)cos(angle[0]);
	constant[1][0] = (float)sin(angle[1]);
	constant[1][1] = (float)cos(angle[1]);
	lookx.x = constant[0][1] * constant[1][1];
	lookx.y = constant[1][0];
	lookx.z = constant[0][0] * constant[1][1];

	lookz.x = -constant[0][0] * constant[1][1];
	lookz.y = constant[1][0];
	lookz.z = constant[0][1] * constant[1][1];

	looky.x = -constant[0][1] * constant[1][0];
	looky.y = constant[1][1];
	looky.z = -constant[0][0] * constant[1][0];
}