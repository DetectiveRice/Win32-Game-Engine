//=============================================================
//Slideshow Application
//Joe Karr
//Created 4-24-2014
//Updated 4-24-2014
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
HINSTANCE		_hInstance;
GameEngine*		_pGame;
const int		_iNUMSLIDES = 7;
Bitmap*			_pSlides[_iNUMSLIDES];
int				_iCurSlide;