//=============================================================
//Slideshow Application
//Joe Karr
//Created 4-24-2014
//Updated 4-24-2014
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "Slideshow.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Slideshow"),
		TEXT("Slideshow"), IDI_SLIDESHOW, IDI_SLIDESHOW_SM);
	if (_pGame == NULL)
		return FALSE;

	//Set the frame rate
	_pGame->SetFrameRate(1);

	//Store the instance handle
	_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	//Create and load the slide bitmaps
	HDC hDC = GetDC(hWindow);
	_pSlides[0] = new Bitmap(hDC, TEXT("Image1.bmp"));
	_pSlides[1] = new Bitmap(hDC, TEXT("Image2.bmp"));
	_pSlides[2] = new Bitmap(hDC, TEXT("Image3.bmp"));
	_pSlides[3] = new Bitmap(hDC, IDB_IMAGE4, _hInstance);
	_pSlides[4] = new Bitmap(hDC, IDB_IMAGE5, _hInstance);
	_pSlides[5] = new Bitmap(hDC, IDB_IMAGE6, _hInstance);
	_pSlides[6] = new Bitmap(hDC, 640, 480, RGB(128, 128, 64));

	//Set the first slide
	_iCurSlide = 0;
}

void GameEnd()
{
	//Cleanup the slide bitmaps
	for (int i = 0; i < _iNUMSLIDES; i++)
		delete _pSlides[i];

	//Cleanup the game engine
	delete _pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
	//Draw the current slide bitmap
	_pSlides[_iCurSlide]->Draw(hDC, 0, 0);
}

void GameCycle()
{
	static int iDelay = 0;

	//Establish a 3-second delay before moving to the next slide
	if (++iDelay > 3)
	{
		//Restore the delay counter
		iDelay = 0;

		//Move to the next slide
		if (++_iCurSlide == _iNUMSLIDES)
			_iCurSlide = 0;

		//Force a repaint to draw the next slide
		InvalidateRect(_pGame->GetWindow(), NULL, FALSE);
	}
}