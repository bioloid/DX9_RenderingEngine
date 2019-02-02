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
	}
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == CRITICAL_DIRECTX_ERROR)
		{
			D3DRelease();
			throw e;
		}
	}


	LPDIRECT3DSURFACE9 originalTarget; // 백 버퍼
	device->GetRenderTarget(0, &originalTarget); // 가져온다..(백업?)
	D3DSURFACE_DESC desc;
	originalTarget->GetDesc(&desc);


	// 렌더타깃을 만든다.
	if (FAILED(device->CreateTexture(desc.Width, desc.Height,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &gpShadowRenderTarget, NULL)))
	{
		console << con::error << con::func << "CreateTexture - shadow map failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	if (FAILED(device->CreateDepthStencilSurface(desc.Width, desc.Height,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&gpShadowDepthStencil, NULL)))
	{
		console << con::error << con::func << "CreateDepthStencilSurface - shadow map failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}



	if (FAILED(device->CreateTexture(desc.Width, desc.Height,
		1, D3DUSAGE_RENDERTARGET, desc.Format,
		D3DPOOL_DEFAULT, &gpRenderTarget, NULL)))
	{
		console << con::error << con::func << "CreateTexture - gpRenderTarget failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	if (FAILED(device->CreateTexture(desc.Width, desc.Height,
		1, D3DUSAGE_RENDERTARGET, desc.Format,
		D3DPOOL_DEFAULT, &VerticalBlurTexture, NULL)))
	{
		console << con::error << con::func << "CreateTexture - gpRenderTarget failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		D3DRelease();
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	if (FAILED(device->CreateTexture(desc.Width, desc.Height,
		1, D3DUSAGE_RENDERTARGET, desc.Format,
		D3DPOOL_DEFAULT, &HorizontalBlurTexture, NULL)))
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
	shader->SetTexture("DiffuseMap_Tex", texture["defaultTexture"]);


	testLight.Initialize(
		D3DXVECTOR4(0.0f, 20.0f, 20.0f, 0.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
		D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), "light_0");

	try
	{
		D3DXMATRIXA16 test_;
		D3DXMatrixIdentity(&test_);
		test_._43 = 12.485;
		screen.Initialize("Screen.obj", test_);
		test_._43 = 0;
		floor.Initialize("Floor.obj", test_);
		test_._41 = 1; test_._43 = 1;
		box0.Initialize("Tree.obj", test_);
		test_._41 = 1; test_._43 = -1;
		box1.Initialize("box2.obj", test_);
		test_._41 = -1; test_._43 = 1;
		box2.Initialize("box2.obj", test_);
		test_._41 = -1; test_._43 = -1;
		box3.Initialize("box2.obj", test_);

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
