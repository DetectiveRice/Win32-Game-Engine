//=============================================================
//Game Skeleton Application
//Joe Karr
//Created 4-23-2014
//Last Edit 4-23-2014
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "Skeleton.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Game Skeleton"),
		TEXT("Game Skeleton"), IDI_SKELETON, IDI_SKELETON_SM);
	if (_pGame == NULL)
		return FALSE;

	//Set the frame rate
	_pGame->SetFrameRate(15);

	return TRUE;
}

void GameStart(HWND hWindow)
{
	//Seed the random number generator
	srand(GetTickCount());
}

void GameEnd()
{
	//Cleanup the game engine
	delete _pGame;
}

void GameActivate(HWND hWindow)
{
	HDC		hDC;
	RECT	rect;

	//Draw activation text on the game screen
	GetClientRect(hWindow, &rect);
	hDC = GetDC(hWindow);
	DrawText(hDC, TEXT("Activated!"), -1, &rect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	ReleaseDC(hWindow, hDC);
}

void GameDeactivate(HWND hWindow)
{
	HDC		hDC;
	RECT	rect;

	// Draw deactivation text on the game screen
	GetClientRect(hWindow, &rect);
	hDC = GetDC(hWindow);
	DrawText(hDC, TEXT("Deactivated!"), -1, &rect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	ReleaseDC(hWindow, hDC);
}

void GamePaint(HDC hDC)
{
}

void GameCycle()
{
	HDC		hDC;
	HWND	hWindow = _pGame->GetWindow();

	//Draw the skeleton icon at random positions on the game screen
	hDC = GetDC(hWindow);
	DrawIcon(hDC, rand() % _pGame->GetWidth(), rand() % _pGame->GetHeight(),
		(HICON)(WORD)GetClassLong(hWindow, GCL_HICON));
	ReleaseDC(hWindow, hDC);
}