#ifndef __TEXTURERENDER_H__
#define __TEXTURERENDER_H__
#include <d3d9.h>

class TEXTURERENDER
{
public:
	void Initialize(bool mode ,int _screenWidth, int _screenHeight, float _near, float _far);
	void SetVB();
	void Release();
	LPDIRECT3DTEXTURE9	renderTraget = NULL;
	D3DXMATRIXA16		orthoMatrix;
	TEXTURERENDER();
	~TEXTURERENDER();
private:
	IDirect3DVertexBuffer9* VB;
};

#endif