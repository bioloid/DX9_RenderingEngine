#include"GAMESYSTEM.h"

void GAMESYSTEM::Render()
{
//	testLight.Rotate();
	testLight.Update();
	camera.Update();
	StartScene();
	{
		LightZBuild();
		
		ShadowBuild();

		DownSamping();

		VerticalBlur();

		HorizontalBlur();

		UpSamping();

		DrawScene();
	}
	EndScene();
//	if (getchar() == 'e')
//		EndGame();
}
