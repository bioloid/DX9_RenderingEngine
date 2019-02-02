#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <d3dx9.h>
#include <string>
using namespace std;
class LIGHT
{
private:



public:
	D3DXMATRIXA16 matLightViewProjection;
	D3DXMATRIXA16 matLightView;
	D3DXMATRIXA16 matLightProjection;

	struct LIGHTDATA
	{
		D3DXVECTOR4 position;
		D3DXVECTOR4 lookTo;
		D3DXVECTOR4 color;
	};
	string		name;
	bool		active;
	LIGHTDATA	data;
	void Initialize(D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, string);
	void Disable(bool);
	void RenderShader();
	void RenderShadow();
	void Release();

	LIGHT();
	~LIGHT();



};
#endif
