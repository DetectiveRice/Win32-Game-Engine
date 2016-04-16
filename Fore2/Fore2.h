//=============================================================
//Fore 2 Application
//Joe Karr
//Created 4-27-2014
//Updated 4-27-2014
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

//=============================================================
//Global Variables
//=============================================================
HINSTANCE	_hInstance;
GameEngine*	_pGame;
HDC			_hOffscreenDC;
HBITMAP		_hOffscreenBitmap;
Bitmap*		_pForestBitmap;
Bitmap*		_pGolfBallBitmap;
Sprite*		_pDragSprite;