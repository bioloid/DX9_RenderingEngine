#ifndef __MODEL_H__
#define __MODEL_H__
#include <d3dx9.h>
#include <string>
#include <vector>
#define OBJ 0
#define ORM 1



// v vt vn
using namespace std;

class MODEL
{
public:
	struct VertexXYZ;
	struct VertexXYZTEX;
	struct VertexXYZNOR;
	struct VertexXYZNORDIFF;
	struct VertexXYZTEXNOR;
private:
	struct MESH;
	struct MTL;
	struct MATRIX2;
	struct MATRIX3;

	string model_name;
	string path;

	int fileType = -1;
	int v, vn, vt;

	vector<MESH> mesh;
	vector<MTL> mtl;

public:
	D3DXMATRIXA16 position;

	static string defaltPath;

	MODEL() {}
	~MODEL() {}

	void Initialize(string, D3DXMATRIXA16);
	void ReadOrm();
	void ReadObj();
	void ReadMtl(string);
	void ReadF(char*, DWORD, int*);
	void ReadFace(char*);
	void Move(D3DXMATRIXA16);
	void Release();

	void RenderShadow(const LPD3DXEFFECT& shader);
	void Render(const LPD3DXEFFECT& shader);


	int  cStrFind(char*, const char*);
	bool bEnable = true;
};

struct MODEL::MESH
{
	IDirect3DVertexBuffer9* VB;
	vector<MTL>::iterator mtl;
	int faceCount;
	DWORD FVF;
};
struct MODEL::MTL
{
	struct MATERIAL
	{
		D3DXVECTOR4 ambient;
		D3DXVECTOR4 diffuse;
		D3DXVECTOR4 specular;
	};
	bool illum;
	IDirect3DTexture9** texture; // ptr
	int blockCode;
	MATERIAL material;
	string* name;
};

struct MODEL::MATRIX2
{
	MATRIX2(float _u, float _v)
	{
		u = _u; v = _v;
	}
	float u, v;
};
struct MODEL::MATRIX3
{
	MATRIX3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}
	float x, y, z;
};
struct MODEL::VertexXYZ
{
	VertexXYZ() {}
	VertexXYZ(float x, float y, float z)
	{
		_x = x;  _y = y;  _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF = D3DFVF_XYZ;
};
struct MODEL::VertexXYZTEX
{
	VertexXYZTEX() {}
	VertexXYZTEX(float x, float y, float z, float u, float v)
	{
		_x = x;  _y = y;  _z = z;
		_u = u;  _v = v;
	}
	float _x, _y, _z, _u, _v;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};
struct MODEL::VertexXYZNOR
{
	VertexXYZNOR() {}
	VertexXYZNOR(float x, float y, float z, float nx, float ny, float nz)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx;  _ny = ny;  _nz = nz;
	}
	float _x, _y, _z, _nx, _ny, _nz;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
};
struct MODEL::VertexXYZNORDIFF
{
	VertexXYZNORDIFF() {}
	VertexXYZNORDIFF(float x, float y, float z, float nx, float ny, float nz, DWORD color)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx;  _ny = ny;  _nz = nz;
		_color = color;
	}
	float _x, _y, _z;
	DWORD _color;
	float _nx, _ny, _nz;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
};
struct MODEL::VertexXYZTEXNOR
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	VertexXYZTEXNOR() {}
	VertexXYZTEXNOR(float x, float y, float z, float u, float v, float nx, float ny, float nz)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx;  _ny = ny;  _nz = nz;
		_u = u;  _v = v;
	}

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
};

#endif