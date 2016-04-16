//=============================================================
//Alien Sprite Object
//Joe Karr
//Created 5-2-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "AlienSprite.h"

//=============================================================
//External Global Variables
//=============================================================
extern Bitmap* _pBlobboBitmap;
extern Bitmap* _pBMissileBitmap;
extern Bitmap* _pJellyBitmap;
extern Bitmap* _pJMissileBitmap;
extern Bitmap* _pTimmyBitmap;
extern Bitmap* _pTMissileBitmap;
extern int	   _iDifficulty;

//=============================================================
//AlienSprite Constructor and Destructor
//=============================================================
AlienSprite::AlienSprite(Bitmap* pBitmap, RECT& rcBounds,
	BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
	baBoundsAction)
{
}

AlienSprite::~AlienSprite()
{
}

//=============================================================
//AlienSprite General Methods
//=============================================================
SPRITEACTION AlienSprite::Update()
{
	//Call the base sprite Update() method
	SPRITEACTION saSpriteAction;
	saSpriteAction = Sprite::Update();

	//See if the alien should fire a missile
	if ((rand() % (_iDifficulty / 2)) == 0)
		saSpriteAction |= SA_ADDSPRITE;

	return saSpriteAction;
}

Sprite* AlienSprite::AddSprite()
{
	//Create a new missile sprite
	RECT		rcBounds = { 0, 0, 640, 410 };
	RECT		rcPos = GetPosition();
	Sprite*		pSprite = NULL;
	if (GetBitmap() == _pBlobboBitmap)
	{
		//Blobbo missile
		pSprite = new Sprite(_pBMissileBitmap, rcBounds, BA_DIE);
		pSprite->SetVelocity(0, 7);
	}
	else if (GetBitmap() == _pJellyBitmap)
	{
		//Jelly Missile
		pSprite = new Sprite(_pJMissileBitmap, rcBounds, BA_DIE);
		pSprite->SetVelocity(0, 5);
	}
	else
	{
		//Timmy missile
		pSprite = new Sprite(_pTMissileBitmap, rcBounds, BA_DIE);
		pSprite->SetVelocity(0, 3);
	}

	//Set the missile sprite's position and return it
	pSprite->SetPosition(rcPos.left + (GetWidth() / 2), rcPos.bottom);
	return pSprite;
}