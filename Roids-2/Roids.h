//=============================================================
//Roids Application
//Joe Karr
//Created 5-1-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Background.h"

//=============================================================
//Global Variables
//=============================================================
HINSTANCE			_hInstance;
GameEngine*			_pGame;
HDC					_hOffscreenDC;
HBITMAP				_hOffscreenBitmap;
Bitmap*				_pAsteroidBitmap;
Bitmap*				_pSaucerBitmap;
Sprite*				_pAsteroids[3];
Sprite*				_pSaucer;
StarryBackground*	_pBackground;

//=============================================================
//Functions Declarations
//=============================================================
void UpdateSaucer();