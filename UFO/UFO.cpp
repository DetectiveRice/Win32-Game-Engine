//=============================================================
//UFO Application
//Joe Karr
//Created 4-25-2014
//Updated 4-25-2014
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "UFO.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("UFO"),
		TEXT("UFO"), IDI_UFO, IDI_UFO_SM, 500, 400);
	if (_pGame == NULL)
		return FALSE;

	//Set the frame rate
	_pGame->SetFrameRate(30);

	//Store the instance handle
	_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	//Create and load the background and saucer bitmaps
	HDC hDC = GetDC(hWindow);
	_pBackground = new Bitmap(hDC, IDB_BACKGROUND, _hInstance);
	_pSaucer[0] = new Bitmap(hDC, IDB_SAUCER, _hInstance);
	_pSaucer[1] = new Bitmap(hDC, IDB_SAUCERFLAME, _hInstance);

	//Set the initial saucer position and speed
	_iSaucerX = 250 - (_pSaucer[0]->GetWidth() / 2);
	_iSaucerY = 200 - (_pSaucer[0]->GetHeight() / 2);
	_iSpeedX = 0;
	_iSpeedY = 0;
}

void GameEnd()
{
	//Cleanup the background and saucer bitmaps
	delete _pBackground;
	delete _pSaucer[0];
	delete _pSaucer[1];

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
	//Draw the background and saucer bitmaps
	_pBackground->Draw(hDC, 0, 0);
	_pSaucer[_pSaucerFlame ? 1:0]->Draw(hDC, _iSaucerX, _iSaucerY, TRUE);
}

void GameCycle()
{
	//Update the saucer position
	_iSaucerX = min(500 - _pSaucer[0]->GetWidth(), max(0, _iSaucerX + _iSpeedX));
	_iSaucerY = min(320, max(0, _iSaucerY + _iSpeedY));

	//Force a repaint to redraw the saucer
	InvalidateRect(_pGame->GetWindow(), NULL, FALSE);
}

void HandleKeys()
{
	//Change the speed of the sauce in response to arrow key presses
	if (GetAsyncKeyState(VK_LEFT) < 0)
		_iSpeedX = max(-_iMAXSPEED, --_iSpeedX);
	else if (GetAsyncKeyState(VK_RIGHT) < 0)
		_iSpeedX = min(_iMAXSPEED, ++_iSpeedX);
	if (GetAsyncKeyState(VK_UP) < 0)
		_iSpeedY = max(-_iMAXSPEED, --_iSpeedY);
	else if (GetAsyncKeyState(VK_DOWN) < 0)
		_iSpeedY = min(_iMAXSPEED, ++_iSpeedY);

	//Turn flame on if arrow press
	_pSaucerFlame = (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT) ||
		GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN));
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	if (bLeft)
	{
		//Set the saucer position to the mouse position
		_iSaucerX = x - (_pSaucer[0]->GetWidth() / 2);
		_iSaucerY = y - (_pSaucer[0]->GetHeight() / 2);
	}
	else
	{
		//Stop the saucer
		_iSpeedX = 0;
		_iSpeedY = 0;
	}
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}