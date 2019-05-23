#include "GAMESYSTEM.h"
#include "TEXTURERENDER.h"


void TEXTURERENDER::Initialize(bool mode, int _screenWidth, int _screenHeight, float _near, float _far)
{
	gSystem.console.SetFunction("TEXTURERENDER::Initialize");
	if (FAILED(gSystem.device->CreateTexture(_screenWidth, _screenHeight,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &renderTraget, NULL)))
	{
		gSystem.console << con::error << con::func << "CreateTexture - renderTraget failed" << con::endl;
		gSystem.console << con::error << con::func << "critical error is detected" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_TEXTURERENDER_CREATETEXTURE_ERROR);
	}



	MODEL::VertexXYZTEX* data_;
	gSystem.device->CreateVertexBuffer(6 * sizeof(MODEL::VertexXYZTEX), D3DUSAGE_WRITEONLY, MODEL::VertexXYZTEX::FVF, D3DPOOL_MANAGED, &VB, 0);
	if (VB == NULL)
	{
		gSystem.console << con::error << con::func << "CreateVertexBuffer() - failed" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_TEXTURERENDER_CREATETEXTURE_ERROR);
	}

	if (mode)
	{
		_screenHeight *= 2;
		_screenWidth *= 2;
	}
	RECT screen;
	screen.top = _screenHeight / 2.0f;
	screen.bottom = _screenHeight / -2.0f;
	screen.right = _screenWidth / 2.0f;
	screen.left = _screenWidth / -2.0f;

	VB->Lock(0, 0, (void**)&data_, 0);
	data_[0] = MODEL::VertexXYZTEX(screen.left, screen.top, 0.0f, 0.0f, 0.0f);
	data_[1] = MODEL::VertexXYZTEX(screen.right, screen.top, 0.0f, 1.0f, 0.0f);
	data_[2] = MODEL::VertexXYZTEX(screen.right, screen.bottom, 0.0f, 1.0f, 1.0f);

	data_[3] = MODEL::VertexXYZTEX(screen.left, screen.top, 0.0f, 0.0f, 0.0f);
	data_[4] = MODEL::VertexXYZTEX(screen.right, screen.bottom, 0.0f, 1.0f, 1.0f);
	data_[5] = MODEL::VertexXYZTEX(screen.left, screen.bottom, 0.0f, 0.0f, 1.0f);
	VB->Unlock();

	D3DXMatrixOrthoRH(&orthoMatrix, _screenWidth, _screenHeight, _near, _far);

	gSystem.console.RestoreFunction();
}

void TEXTURERENDER::SetVB()
{
	if (gSystem.savedFVF != MODEL::VertexXYZTEX::FVF)
	{
		gSystem.device->SetFVF(MODEL::VertexXYZTEX::FVF);
		gSystem.savedFVF = MODEL::VertexXYZTEX::FVF;
	}
	gSystem.device->SetStreamSource(0, VB, 0, sizeof(MODEL::VertexXYZTEX));
}
void TEXTURERENDER::Release()
{
	renderTraget->Release();
	VB->Release();
}
TEXTURERENDER::TEXTURERENDER()
{
}


TEXTURERENDER::~TEXTURERENDER()
{
}
