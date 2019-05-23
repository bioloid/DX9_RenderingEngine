#include "GAMESYSTEM.h"
#define SCREEN_X 1.0f / gSystem.winSize.right
#define SCREEN_Y 1.0f / gSystem.winSize.bottom
void GAMESYSTEM::Test()
{
	if (SUCCEEDED(shadowVerticalBlurRT->GetSurfaceLevel(0, &tmpSurface)))
	{
		device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	shadowVerticalBlurShader->SetMatrix("OrthoMatrix", &downSampler.orthoMatrix);
	downSampler.SetVB();
	shadowVerticalBlurShader->SetTexture("BlurTexture", shadowHorizontalBlurRT);
	shadowHorizontalBlurShader->SetRawValue("Screen", &D3DXVECTOR2(SCREEN_X, SCREEN_Y), 0, sizeof(D3DXVECTOR2));

	device->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);

	shadowVerticalBlurShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			shadowVerticalBlurShader->BeginPass(i);
			{
				gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
			}
			shadowVerticalBlurShader->EndPass();
		}
	}
	shadowVerticalBlurShader->End();



	if (SUCCEEDED(shadowHorizontalBlurRT->GetSurfaceLevel(0, &tmpSurface)))
	{
		device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	shadowHorizontalBlurShader->SetMatrix("OrthoMatrix", &downSampler.orthoMatrix);
	downSampler.SetVB();
	shadowHorizontalBlurShader->SetTexture("BlurTexture", shadowVerticalBlurRT);
	shadowHorizontalBlurShader->SetRawValue("Screen", &D3DXVECTOR2(SCREEN_X, SCREEN_Y), 0, sizeof(D3DXVECTOR2));

	device->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);

	shadowHorizontalBlurShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			shadowHorizontalBlurShader->BeginPass(i);
			{
				gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
			}
			shadowHorizontalBlurShader->EndPass();
		}
	}
	shadowHorizontalBlurShader->End();

}
void GAMESYSTEM::UpSamping()
{
	if (SUCCEEDED(upSampler.renderTraget->GetSurfaceLevel(0, &tmpSurface)))
	{
		gSystem.device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}

	SamplingShader->SetMatrix("OrthoMatrix", &upSampler.orthoMatrix);
	SamplingShader->SetTexture("SamplingTexture", shadowHorizontalBlurRT);
	upSampler.SetVB();

	gSystem.device->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);
	UINT shaderNumPass;
	SamplingShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			SamplingShader->BeginPass(i);
			{
				gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
			}
			SamplingShader->EndPass();
		}
	}
	SamplingShader->End();
//	D3DXSaveTextureToFile("6.bmp", D3DXIFF_BMP, upSampler.renderTraget, NULL);
}

void GAMESYSTEM::DownSamping()
{
	if (SUCCEEDED(downSampler.renderTraget->GetSurfaceLevel(0, &tmpSurface)))
	{
		gSystem.device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	SamplingShader->SetMatrix("OrthoMatrix", &downSampler.orthoMatrix);
	SamplingShader->SetTexture("SamplingTexture", shadowBlackWhiteBuildRT);
	downSampler.SetVB();

	gSystem.device->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);
	UINT shaderNumPass;
	SamplingShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			SamplingShader->BeginPass(i);
			{
				gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
			}
			SamplingShader->EndPass();
		}
	}
	SamplingShader->End();

//	D3DXSaveTextureToFile("3.bmp", D3DXIFF_BMP, downSampler.renderTraget, NULL);
}
void GAMESYSTEM::LightZBuild()
{
	if (SUCCEEDED(shadowZBuildRT->GetSurfaceLevel(0, &tmpSurface)))
	{
		device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	device->SetDepthStencilSurface(shadowZBuildStencil);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , 0xFFFFFFFF, 1.0f, 0);
	//D3DCLEAR_STENCIL
	testLight.SetViewProjectionMatrix(shadowZBuildShader);

	shadowZBuildShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			shadowZBuildShader->BeginPass(i);
			{
				box0.Render(shadowZBuildShader);
				box1.Render(shadowZBuildShader);
				box2.Render(shadowZBuildShader);
				box3.Render(shadowZBuildShader);
				floor.Render(shadowZBuildShader);
			}
			shadowZBuildShader->EndPass();
		}
	}
	shadowZBuildShader->End();
}
void GAMESYSTEM::ShadowBuild()
{
	if (SUCCEEDED(shadowBlackWhiteBuildRT->GetSurfaceLevel(0, &tmpSurface)))
	{
		device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	camera.SetViewProjectionMatrix(shadowBlackWhiteBuildShader);
	testLight.SetViewProjectionMatrix(shadowBlackWhiteBuildShader);
	shadowBlackWhiteBuildShader->SetTexture("ShadowTexture", shadowZBuildRT);
	shadowBlackWhiteBuildShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			shadowBlackWhiteBuildShader->BeginPass(i);
			{
				box0.Render(shadowBlackWhiteBuildShader);
				box1.Render(shadowBlackWhiteBuildShader);
				box2.Render(shadowBlackWhiteBuildShader);
				box3.Render(shadowBlackWhiteBuildShader);
				floor.Render(shadowBlackWhiteBuildShader);
			}
			shadowBlackWhiteBuildShader->EndPass();
		}
	}
	shadowBlackWhiteBuildShader->End();
//	D3DXSaveTextureToFile("2.bmp", D3DXIFF_BMP, shadowBlackWhiteBuildRT, NULL);
}
void GAMESYSTEM::VerticalBlur()
{
	if (SUCCEEDED(shadowVerticalBlurRT->GetSurfaceLevel(0, &tmpSurface)))
	{
		device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	shadowVerticalBlurShader->SetMatrix("OrthoMatrix", &downSampler.orthoMatrix);
	downSampler.SetVB();
	shadowVerticalBlurShader->SetTexture("BlurTexture", downSampler.renderTraget);
	shadowHorizontalBlurShader->SetRawValue("Screen", &D3DXVECTOR2(SCREEN_X, SCREEN_Y), 0, sizeof(D3DXVECTOR2));

	device->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);

	shadowVerticalBlurShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			shadowVerticalBlurShader->BeginPass(i);
			{
				gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
			}
			shadowVerticalBlurShader->EndPass();
		}
	}
	shadowVerticalBlurShader->End();

//	D3DXSaveTextureToFile("4.bmp", D3DXIFF_BMP, shadowVerticalBlurRT, NULL);

}

void GAMESYSTEM::HorizontalBlur()
{
	if (SUCCEEDED(shadowHorizontalBlurRT->GetSurfaceLevel(0, &tmpSurface)))
	{
		device->SetRenderTarget(0, tmpSurface);
		tmpSurface->Release();
		tmpSurface = NULL;
	}
	shadowHorizontalBlurShader->SetMatrix("OrthoMatrix", &downSampler.orthoMatrix);
	downSampler.SetVB();
	shadowHorizontalBlurShader->SetTexture("BlurTexture", shadowVerticalBlurRT);
	shadowHorizontalBlurShader->SetRawValue("Screen", &D3DXVECTOR2(SCREEN_X, SCREEN_Y), 0, sizeof(D3DXVECTOR2));

	device->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);

	shadowHorizontalBlurShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			shadowHorizontalBlurShader->BeginPass(i);
			{
				gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);
			}
			shadowHorizontalBlurShader->EndPass();
		}
	}
	shadowHorizontalBlurShader->End();
//	D3DXSaveTextureToFile("5.bmp", D3DXIFF_BMP, shadowHorizontalBlurRT, NULL);
}



void GAMESYSTEM::DrawScene()
{
	device->SetRenderTarget(0, backUpMainRT);
	device->SetDepthStencilSurface(bakcUpMainStencil);
	camera.SetViewProjectionMatrix(lastSceneShader);
	camera.SetPositionVector(lastSceneShader);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	testLight.SetViewProjectionMatrix(lastSceneShader);
	testLight.SetData(lastSceneShader);
//	lastSceneShader->SetTexture("BlurTexture", upSampler.renderTraget);
	lastSceneShader->SetTexture("BlurTexture", shadowBlackWhiteBuildRT);

	
	lastSceneShader->Begin(&shaderNumPass, NULL);
	{
		for (UINT i = 0; i < shaderNumPass; i++)
		{
			lastSceneShader->BeginPass(i);
			{
				box0.Render(lastSceneShader);
				box1.Render(lastSceneShader);
				box2.Render(lastSceneShader);
				box3.Render(lastSceneShader);
				floor.Render(lastSceneShader);
			}
			lastSceneShader->EndPass();
		}
	}
	lastSceneShader->End();
	DrawMSG();
}

void GAMESYSTEM::StartScene()
{
	device->GetRenderTarget(0, &backUpMainRT);
	device->GetDepthStencilSurface(&bakcUpMainStencil);
	device->BeginScene();
}
void GAMESYSTEM::EndScene()
{
	device->EndScene();
	device->Present(0, 0, 0, 0);
	backUpMainRT->Release();
	bakcUpMainStencil->Release();
}