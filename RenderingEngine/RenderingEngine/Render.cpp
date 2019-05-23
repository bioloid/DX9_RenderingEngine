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

	//	DownSamping();

	//	VerticalBlur();

	//	HorizontalBlur();
	//	for (int i = 0; i < 0; i++)
	//		Test();

	//	UpSamping();

		DrawScene();
	}
	EndScene();
//	if (getchar() == 'e')
//		EndGame();
}
