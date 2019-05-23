#include "CAMERA.h"
#include "GAMESYSTEM.h"

CAMERA::CAMERA()
{
}

CAMERA::~CAMERA()
{
}
void CAMERA::Backup()
{
	bPosition = position;
	bAngle = angle;
}
void CAMERA::Restore()
{
	position = bPosition;
	angle[0] = bAngle.x;
	angle[1] = bAngle.y;
}
void CAMERA::Initialize()
{
	length = 1.0f; // distance between camera position and target

	speed.s = 0.01f;
	speed.pi = speed.th = 0.001f;

	near_ = 1.0f; far_ = 1000.0f;

	fov = 1.047183f;	// 60 degree in radian

	angle[0] = 1.5707745f; // 90 degree in radian
	angle[1] = -0.5f;

	position = { 0 , 5 , -10 };
	up = { 0.0f, 1.0f, 0.0f };
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
}


void CAMERA::SetViewProjectionMatrix(const LPD3DXEFFECT& shader)
{

	shader->SetMatrix("ViewProjectionMatrix", &viewProjectionMatrix);
}

void CAMERA::SetPositionVector(const LPD3DXEFFECT & shader)
{
	shader->SetVector("CameraPosition", &D3DXVECTOR4(position));
}

void CAMERA::Update()
{
	CalculateConst();
	CalculateMatrix();
}

void CAMERA::MouseMove(int _moveX, int _moveY)
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
	}
}

void CAMERA::CalculateMatrix()
{
	
	target = position + lookx * length;
	D3DXMatrixLookAtLH(&viewMatrix, &position, &target, &up);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, near_, far_);
	viewProjectionMatrix = viewMatrix * projectionMatrix;
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