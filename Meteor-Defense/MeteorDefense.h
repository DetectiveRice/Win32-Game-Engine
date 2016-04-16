//=============================================================
//Meteor Defense Application
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
Bitmap*				_pGroundBitmap;
Bitmap*				_pTargetBitmap;
Bitmap*				_pCityBitmap;
Bitmap*				_pMeteorBitmap;
Bitmap*				_pMissileBitmap;
Bitmap*				_pExplosionBitmap;
Bitmap*				_pGameOverBitmap;
StarryBackground*	_pBackground;
Sprite*				_pTargetSprite;
int					_iNumCities, _iScore, _iDifficulty;
BOOL				_bGameOver;

//=============================================================
//Function Declarations
//=============================================================
void	 NewGame();
void	 AddMeteor();