#include"GAMESYSTEM.h"

void GAMESYSTEM::Render()
{
	static float count = 0.0f;
	count += 0.004f;
	if (count > 2 * 3.14f)
	{
		count = 0;
	}
	testLight.data.position = { 10 * cos(count), 5, 10 * sin(count), 1 };


	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	UINT numPasses;

	device->GetRenderTarget(0, &pHWBackBuffer);
	device->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	if (SUCCEEDED(gpShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		device->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	device->SetDepthStencilSurface(gpShadowDepthStencil);

	device->BeginScene();
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);


	testLight.RenderShadow();

	shadow->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			shadow->BeginPass(i);
			{
				box0.ShadowRender();
				box1.ShadowRender();
				box2.ShadowRender();
				box3.ShadowRender();
				floor.ShadowRender();
			}
			shadow->EndPass();
		}
	}
	shadow->End();


	if (SUCCEEDED(gpRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		device->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	camera.RenderSoftShadow();
	SoftShadow->SetMatrix("LightViewProjectionMatrix", &testLight.matLightViewProjection);
	SoftShadow->SetTexture("ShadowTexture", gpShadowRenderTarget);
	SoftShadow->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			SoftShadow->BeginPass(i);
			{
				box0.SoftShadow();
				box1.SoftShadow();
				box2.SoftShadow();
				box3.SoftShadow();
				floor.SoftShadow();
			}
			SoftShadow->EndPass();
		}
	}
	SoftShadow->End();
//	D3DXSaveTextureToFile("2.png", D3DXIFF_BMP, gpRenderTarget, NULL);

	camera.Backup();
	camera.position.x = 0;
	camera.position.y = 0;
	camera.position.z = 0;
	camera.angle[0] = 3.14 / 2;
	camera.angle[1] = 0;
	camera.RenderSoftShadow();

	if (SUCCEEDED(VerticalBlurTexture->GetSurfaceLevel(0, &pShadowSurface)))
	{
		device->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	VerticalShader->SetMatrix("ViewProjectionMatrix", &camera.gMatViewProjection);
	VerticalShader->SetTexture("BlurTexture", gpRenderTarget);
	VerticalShader->SetRawValue("Screen", &D3DXVECTOR2(1.0f/1280.0f, 1.0f / 720.0f), 0, sizeof(D3DXVECTOR2));
	
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	VerticalShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			VerticalShader->BeginPass(i);
			{
				VerticalShader->SetMatrix("WorldMatrix", &screen.position);
				VerticalShader->CommitChanges();
				screen.RenderVertical();
			}
			VerticalShader->EndPass();
		}
	}
	VerticalShader->End();
//	D3DXSaveTextureToFile("3.png", D3DXIFF_BMP, VerticalBlurTexture, NULL);


	if (SUCCEEDED(HorizontalBlurTexture->GetSurfaceLevel(0, &pShadowSurface)))
	{
		device->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	HorizontalShader->SetMatrix("ViewProjectionMatrix", &camera.gMatViewProjection);
	HorizontalShader->SetTexture("BlurTexture", VerticalBlurTexture);
	HorizontalShader->SetRawValue("Screen", &D3DXVECTOR2(1.0f / 1280.0f, 1.0f / 720.0f), 0, sizeof(D3DXVECTOR2));

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	HorizontalShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			HorizontalShader->BeginPass(i);
			{
				HorizontalShader->SetMatrix("WorldMatrix", &screen.position);
				HorizontalShader->CommitChanges();
				screen.RenderVertical();
			}
			HorizontalShader->EndPass();
		}
	}
	HorizontalShader->End();
//	D3DXSaveTextureToFile("4.png", D3DXIFF_BMP, HorizontalBlurTexture, NULL);


	device->SetRenderTarget(0, pHWBackBuffer);
	device->SetDepthStencilSurface(pHWDepthStencilBuffer);
	pHWBackBuffer->Release();
	pHWDepthStencilBuffer->Release();
	camera.Restore();
	camera.RenderSoftShadow();


	device->BeginScene();
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	MainShader->SetMatrix("LightViewProjectionMatrix", &testLight.matLightViewProjection);
	MainShader->SetRawValue("light_0", &testLight.data, 0, sizeof(LIGHT::LIGHTDATA));
	MainShader->SetTexture("BlurTexture", HorizontalBlurTexture);
	MainShader->SetMatrix("ViewProjectionMatrix", &camera.gMatViewProjection);
	MainShader->SetVector("CameraPosition", &D3DXVECTOR4(camera.position));

	DrawMSG();
	MainShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			MainShader->BeginPass(i);
			{
				box0.test3();
				box1.test3();
				box2.test3();
				box3.test3();
				floor.test3();
			}
			MainShader->EndPass();
		}
	}
	MainShader->End();

	device->EndScene();
	device->Present(0, 0, 0, 0);
//	EndGame();
}
