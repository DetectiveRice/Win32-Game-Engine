//=============================================================
//Fore Application
//Joe Karr
//Created 4-26-2014
//Updated 4-26-2014
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
GameEngine*	_pGame;
Bitmap*		_pForestBitmap;
Bitmap*		_pGolfBallBitmap;
Sprite*		_pGolfBallSprite[3];
BOOL		_bDragging;
int			_iDragBall;