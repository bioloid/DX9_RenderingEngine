#include "LIGHT.h"
#include "GAMESYSTEM.h"



void LIGHT::Initialize(D3DXVECTOR4 _pos, D3DXVECTOR4 _lookTo, D3DXVECTOR4 _color, string _name)
{
	data.position = _pos;
	data.lookTo = _lookTo;
	name = _name;
	active = true;

	D3DXMatrixLookAtLH(
		&View, &D3DXVECTOR3(data.position),
		&D3DXVECTOR3(data.lookTo), &gSystem.camera.up);
	D3DXMatrixPerspectiveFovLH(&Projection, gSystem.camera.fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, gSystem.camera.near_, gSystem.camera.far_);
	ViewProjectionMatrix = View * Projection;
}
void LIGHT::Disable(bool _active)
{
	active = _active;
}
void LIGHT::Rotate()
{
	rotateCount += 0.004f;
	if (rotateCount > 2 * 3.14f)
	{
		rotateCount = 0;
	}
	data.position = { 10 * cos(rotateCount), 5, 10 * sin(rotateCount), 1 };
}
void LIGHT::Update()
{
	if (active)
	{
		D3DXMatrixLookAtLH(
			&View, &D3DXVECTOR3(data.position),
			&D3DXVECTOR3(data.lookTo), &gSystem.camera.up);
		D3DXMatrixPerspectiveFovLH(&Projection, gSystem.camera.fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, gSystem.camera.near_, gSystem.camera.far_);
		ViewProjectionMatrix = View * Projection;
	}

}
void LIGHT::SetViewProjectionMatrix(const LPD3DXEFFECT& shader)
{
	if (active)
	{
		shader->SetMatrix("LightViewProjectionMatrix", &ViewProjectionMatrix);
	}
}

void LIGHT::SetData(const LPD3DXEFFECT & shader)
{
	if (active)
	{
		shader->SetRawValue(name.c_str(), &data, 0, sizeof(DATA));
	}
}

void LIGHT::Release()
{
}


LIGHT::LIGHT()
{
}

LIGHT::~LIGHT()
{
}
