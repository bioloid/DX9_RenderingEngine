#include "LIGHT.h"
#include "GAMESYSTEM.h"



void LIGHT::Initialize(D3DXVECTOR4 _pos, D3DXVECTOR4 _lookTo, D3DXVECTOR4 _color, string _name)
{
	data.position = _pos;
	data.lookTo = _lookTo;
	data.color = _color;
	name = _name;
	active = true;

	D3DXMatrixLookAtLH(
		&matLightView, &D3DXVECTOR3(data.position),
		&D3DXVECTOR3(data.lookTo), &gSystem.camera.up);
	D3DXMatrixPerspectiveFovLH(&matLightProjection, gSystem.camera.fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, gSystem.camera.near_, gSystem.camera.far_);
	matLightViewProjection = matLightView * matLightProjection;
}
void LIGHT::Disable(bool _active)
{
	sizeof(D3DXVECTOR4);
	active = _active;
}

void LIGHT::RenderShader()
{
	D3DXMatrixLookAtLH(
		&matLightView, &D3DXVECTOR3(data.position),
		&D3DXVECTOR3(data.lookTo), &gSystem.camera.up);
	D3DXMatrixPerspectiveFovLH(&matLightProjection, gSystem.camera.fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, gSystem.camera.near_, gSystem.camera.far_);
	matLightViewProjection = matLightView * matLightProjection;
	gSystem.shader->SetMatrix("gMatLightViewProjection", &matLightViewProjection);
	gSystem.shader->SetRawValue(name.c_str(), &data, 0, sizeof(LIGHTDATA));
}

void LIGHT::RenderShadow()
{
	D3DXMatrixLookAtLH(
		&matLightView, &D3DXVECTOR3(data.position),
		&D3DXVECTOR3(data.lookTo), &gSystem.camera.up);
	D3DXMatrixPerspectiveFovLH(&matLightProjection, gSystem.camera.fov, (float)gSystem.winSize.right / (float)gSystem.winSize.bottom, gSystem.camera.near_, gSystem.camera.far_);
	matLightViewProjection = matLightView * matLightProjection;
	gSystem.shadow->SetMatrix("gMatLightViewProjection", &matLightViewProjection);
	gSystem.shadow->SetRawValue(name.c_str(), &data, 0, sizeof(LIGHTDATA));
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
