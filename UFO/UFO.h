//=============================================================
//UFO Applicatoin
//Joe Karr
//Created 4-25-2014
//Updated 4-25-2014
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

//=============================================================
//Global Variables
//=============================================================
HINSTANCE	_hInstance;
GameEngine* _pGame;
const int	_iMAXSPEED = 8;
Bitmap*		_pBackground;
Bitmap*		_pSaucer[2];
BOOL		_pSaucerFlame;
int			_iSaucerX, _iSaucerY;
int			_iSpeedX, _iSpeedY;