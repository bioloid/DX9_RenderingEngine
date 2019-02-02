#include"GAMESYSTEM.h"

void GAMESYSTEM::Render()
{
	static float count = 3.14 * 1.5;
//	count += 0.004;
	if (count > 2 * 3.14)
		count = 0;
	testLight.data.position = { 10*cos(count), 10, 10*sin(count), 1 };


	camera.SetPosition();
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
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

	UINT numPasses;
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
	device->EndScene();

	LPDIRECT3DSURFACE9 ShadowSurface = NULL;
	if (SUCCEEDED(gpRenderTarget->GetSurfaceLevel(0, &ShadowSurface)))
	{
		device->SetRenderTarget(0, ShadowSurface);
		ShadowSurface->Release();
		ShadowSurface = NULL;
	}

	device->BeginScene();
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);


	camera.SetPosition();
	testLight.RenderShader();
	shader->SetVector("gWorldCameraPosition", &D3DXVECTOR4(camera.position, 1.0f));
	shader->SetTexture("ShadowTexture", gpShadowRenderTarget);
	DrawMSG();
	gSystem.shader->SetTechnique("VTN");

	gSystem.shader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			gSystem.shader->BeginPass(i);
			{
				box0.Render();
				box1.Render();
				box2.Render();
				box3.Render();
				floor.Render();
			}
			gSystem.shader->EndPass();
		}
	}
	gSystem.shader->End();
	device->EndScene();

	device->SetRenderTarget(0, pHWBackBuffer);
	device->SetDepthStencilSurface(pHWDepthStencilBuffer);
	pHWBackBuffer->Release();
	pHWDepthStencilBuffer->Release();

	device->BeginScene();
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);


	camera.SetPosition();
//	D3DXSaveTextureToFile("test.png", D3DXIFF_BMP, gpRenderTarget, NULL);
	shader->SetTexture("gBlurTexture", gpRenderTarget);
	gSystem.shader->SetTechnique("Blur");

	gSystem.shader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; i++)
		{
			gSystem.shader->BeginPass(i);
			{
				screen.Test();
			}
			gSystem.shader->EndPass();
		}
	}
	gSystem.shader->End();

	device->EndScene();
	device->Present(0, 0, 0, 0);
//	EndGame();
}
