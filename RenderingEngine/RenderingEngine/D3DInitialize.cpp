#include "GAMESYSTEM.h"

void GAMESYSTEM::D3DInitialize(bool _windowed, D3DDEVTYPE _deviceType)
{
	console.SetFunction("D3DInitialize");

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		console << con::error << con::func << "Direct3DCreate9() - failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, _deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, _deviceType, D3DFMT_A8R8G8B8, false, D3DMULTISAMPLE_16_SAMPLES, NULL);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = winSize.right;
	d3dpp.BackBufferHeight = winSize.bottom;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = _windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//D3DPRESENT_INTERVAL_IMMEDIATE
	//D3DPRESENT_INTERVAL_DEFAULT
	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, _deviceType, hwnd, vp, &d3dpp, &device)))
	{
		console << con::error << con::func << "CreateDevice() - D3DFMT_D24S8 failed" << con::endl;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16; // 16 - bit Buffer
		if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, _deviceType, hwnd, vp, &d3dpp, &device)))
		{
			d3d9->Release();
			console << con::error << con::func << "CreateDevice() - D3DFMT_D16 failed" << con::endl;
			console << con::error << con::func << "critical error is detected" << con::endl;
			throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
		}
	}
	d3d9->Release();
	D3DCAPS9 Caps;
	device->GetDeviceCaps(&Caps);
	cout << "MRT : " << Caps.NumSimultaneousRTs << endl;


	D3DXCreateFont(device, 15, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "system", &font);

	//*********************************//
	//	Shader Load					   //
	//*********************************//
	try
	{
		D3DEffectInitialize();
		downSampler.Initialize(false, winSize.right, winSize.bottom, 0.0f, 1.0f);
		upSampler.Initialize(false, winSize.right, winSize.bottom, 0.0f, 1.0f);
	}
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == CRITICAL_DIRECTX_ERROR)
		{
			D3DRelease();
			throw e;
		}
		if (e.error == CRITICAL_DIRECTX_TEXTURERENDER_CREATETEXTURE_ERROR)
		{
			getchar();
			D3DRelease();
			throw e;
		}
	}




	// ����Ÿ���� �����.
	const int size = 512;
	if (FAILED(device->CreateTexture(size, size,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &shadowZBuildRT, NULL)))
	{
		console << con::error << con::func << "CreateTexture - shadow map failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	// �׸��� �ʰ� ������ ũ���� ���̹��۵� �������� �Ѵ�.
	if (FAILED(device->CreateDepthStencilSurface(size, size,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &shadowZBuildStencil, NULL)))
	{
		console << con::error << con::func << "CreateDepthStencilSurface - shadow map failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}



	if (FAILED(device->CreateTexture(size, size,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &shadowBlackWhiteBuildRT, NULL)))
	{
		console << con::error << con::func << "CreateTexture - gpRenderTarget failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	if (FAILED(device->CreateTexture(winSize.right, winSize.bottom,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &shadowVerticalBlurRT, NULL)))
	{
		console << con::error << con::func << "CreateTexture - gpRenderTarget failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	if (FAILED(device->CreateTexture(winSize.right, winSize.bottom,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &shadowHorizontalBlurRT, NULL)))
	{
		console << con::error << con::func << "CreateTexture - gpRenderTarget failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	camera.Initialize();


	texture["defaultTexture"] = NULL;
	if (FAILED(D3DXCreateTextureFromFile
	(device, "Data\\Default\\Texture\\defaultTexture.png", &texture["defaultTexture"])))
	{
		console << con::error << con::func << "D3DXCreateTextureFromFile() - default error" << con::endl;
		console << con::error << con::func << "check default texture file" << con::endl;
		console << con::error << con::func << "path : Data\\Default\\Texture\\defaultTexture.png" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}
	default_texture = &texture["defaultTexture"];


	testLight.Initialize(
		D3DXVECTOR4(0.0f, 20.0f, 10.0f, 0.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
		D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "light_0");

	try
	{
		D3DXMATRIXA16 test_;
		D3DXMatrixIdentity(&test_);
		test_._43 = 0;
		floor.Initialize("Floor.obj", test_);
		test_._41 = 1; test_._43 = 1;
		box0.Initialize("cube.obj", test_);
		test_._41 = 1; test_._43 = -1;
		box1.Initialize("cube.obj", test_);
		test_._41 = -1; test_._43 = 1;
		box2.Initialize("cube.obj", test_);
		test_._41 = -1; test_._43 = -1;
		box3.Initialize("cube.obj", test_);

		//	test.Initialize("lowpolytree.obj", test_);
		//	test.Initialize("test.obj", test_);
		//	test.Initialize("wooden watch tower2.obj", test_);
		//	test.Initialize("MaleLow.obj", test_);
		//	test.Initialize("low-poly-mill.obj", test_);
		//	test.Initialize("IronMan.obj", test_);
		//	test.Initialize("v.obj", test_);
	}
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == MODEL_LOAD_ERROR)
		{
			cout << "error" << endl;
		}
	}


//	D3D Render Setting
	{
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}


	avgfps.Initialize(AVG);
	instfps.Initialize(INST);
	console.RestoreFunction();
}
