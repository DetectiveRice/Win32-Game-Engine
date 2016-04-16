//=============================================================
//Alien Sprite Object
//Joe Karr
//Created 5-2-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>
#include "Sprite.h"

//=============================================================
//AlienSprite Class
//=============================================================
class AlienSprite : public Sprite
{
public:
	//Constructor and Destructor
	AlienSprite(Bitmap* pBitmap, RECT& rcBounds,
		BOUNDSACTION baBoundsAction = BA_STOP);
	
	virtual ~AlienSprite();

	//General Methods
	virtual SPRITEACTION Update();
	virtual Sprite*		 AddSprite();
};