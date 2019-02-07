#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <d3dx9.h>
#include <string>
using namespace std;
class LIGHT
{
private:



public:
	D3DXMATRIXA16 ViewProjectionMatrix;
	D3DXMATRIXA16 View;
	D3DXMATRIXA16 Projection;
	float rotateCount = 0.0f;
	struct DATA
	{
		D3DXVECTOR4 position;
		D3DXVECTOR4 lookTo;
	};
	string		name;
	bool		active;
	DATA	data;
	void Initialize(D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, string);
	void Disable(bool);
	void Rotate();
	void Update();
	void SetViewProjectionMatrix(const LPD3DXEFFECT& shader);
	void SetData(const LPD3DXEFFECT& shader);
	void Release();
	LIGHT();
	~LIGHT();



};
#endif
