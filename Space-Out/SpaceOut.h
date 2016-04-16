//=============================================================
//Space Out Application
//Joe Karr
//Created 5-2-2014
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
#include "AlienSprite.h"

//=============================================================
//Global Variables
//=============================================================
HINSTANCE			_hInstance;
GameEngine*			_pGame;
HDC					_hOffscreenDC;
HBITMAP				_hOffscreenBitmap;
Bitmap*				_pDesertBitmap;
Bitmap*				_pCarBitmap;
Bitmap*				_pSmCarBitmap;
Bitmap*				_pMissileBitmap;
Bitmap*				_pBlobboBitmap;
Bitmap*				_pBMissileBitmap;
Bitmap*				_pJellyBitmap;
Bitmap*				_pJMissileBitmap;
Bitmap*				_pTimmyBitmap;
Bitmap*				_pTMissileBitmap;
Bitmap*				_pSmExplosionBitmap;
Bitmap*				_pLgExplosionBitmap;
Bitmap*				_pGameOverBitmap;
Bitmap*				_pSplashBitmap;
StarryBackground*	_pBackground;
Sprite*				_pCarSprite;
int					_iFireInputDelay;
int					_iNumLives, _iScore, _iDifficulty;
int					_iGameOverDelay;
BOOL				_bGameOver;
BOOL				_bDemo;

//=============================================================
//Function Declarations
//=============================================================
void NewGame();
void AddAlien();