//=============================================================
//Brainiac Application
//Joe Karr
//Created 4-25-2014
//Updated 4-25-20144
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

//=============================================================
//Global Variables
//=============================================================
HINSTANCE		_hInstance;
GameEngine*		_pGame;
Bitmap*			_pTiles[9];
BOOL			_bTileStates[4][4];
int				_iTiles[4][4];
POINT			_ptTile1, _ptTile2;
int				_iMatches, _iTries;