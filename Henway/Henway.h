//=============================================================
//Henway Application
//Joe Karr
//Created 4-27-2014
//Updated 4-27-2014
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

//=============================================================
//Global Variables
//=============================================================
HINSTANCE	_hInstance;
GameEngine* _pGame;
HDC			_hOffscreenDC;
HBITMAP		_hOffscreenBitmap;
Bitmap*		_pHighwayBitmap;
Bitmap*		_pChickenBitmap;
Bitmap*		_pCarBitmaps[4];
Bitmap*		_pChickenHeadBitmap;
Sprite*		_pChickenSprite;
int			_iInputDelay;
int			_iNumLives;
int			_iScore;
BOOL		_bGameOver;

//=============================================================
//Funcation Declarations
//=============================================================
void	MoveChicken(int iXDistance, int iYDistance);