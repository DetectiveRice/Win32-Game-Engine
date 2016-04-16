//=============================================================
//Roids Application
//Joe Karr
//Created 5-1-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "Roids.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Roids"),
		TEXT("Roids"), IDI_ROIDS, IDI_ROIDS_SM, 500, 400);
	if (_pGame == NULL)
		return FALSE;

	//Set the frame rate
	_pGame->SetFrameRate(30);

	//Store the instance handle
	_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	//Seed the random number generator
	srand(GetTickCount());

	//Create the offscreen device context and bitmap
	_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
		_pGame->GetWidth(), _pGame->GetHeight());
	SelectObject(_hOffscreenDC, _hOffscreenBitmap);

	//Create and load the asteroid bitmap
	HDC hDC = GetDC(hWindow);
	_pAsteroidBitmap = new Bitmap(hDC, IDB_ASTEROID, _hInstance);
	_pSaucerBitmap = new Bitmap(hDC, IDB_SAUCER, _hInstance);

	//Create the starry background
	_pBackground = new StarryBackground(500, 400);

	//Create the asteroid sprites
	RECT	rcBounds = { 0, 0, 500, 400 };

	_pAsteroids[0] = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
	_pAsteroids[0]->SetNumFrames(14);
	_pAsteroids[0]->SetFrameDelay(1);
	_pAsteroids[0]->SetPosition(250, 200);
	_pAsteroids[0]->SetVelocity(-3, 1);
	_pGame->AddSprite(_pAsteroids[0]);

	_pAsteroids[1] = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
	_pAsteroids[1]->SetNumFrames(14);
	_pAsteroids[1]->SetFrameDelay(2);
	_pAsteroids[1]->SetPosition(250, 200);
	_pAsteroids[1]->SetVelocity(3, -2);
	_pGame->AddSprite(_pAsteroids[1]);

	_pAsteroids[2] = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
	_pAsteroids[2]->SetNumFrames(14);
	_pAsteroids[2]->SetFrameDelay(3);
	_pAsteroids[2]->SetPosition(250, 200);
	_pAsteroids[2]->SetVelocity(-2, -4);
	_pGame->AddSprite(_pAsteroids[2]);

	//Create the saucer sprite
	_pSaucer = new Sprite(_pSaucerBitmap, rcBounds, BA_WRAP);
	_pSaucer->SetPosition(100, 100);
	_pSaucer->SetVelocity(3, 1);
	_pGame->AddSprite(_pSaucer);
}

void GameEnd()
{
  // Cleanup the offscreen device context and bitmap
  DeleteObject(_hOffscreenBitmap);
  DeleteDC(_hOffscreenDC);  

  // Cleanup the asteroid bitmap
  delete _pAsteroidBitmap;

  // Cleanup the background
  delete _pBackground;

  // Cleanup the sprites
  _pGame->CleanupSprites();

  // Cleanup the game engine
  delete _pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
  // Draw the background
  _pBackground->Draw(hDC);

  // Draw the sprites
  _pGame->DrawSprites(hDC);
}

void GameCycle()
{
	//Update the background
	_pBackground->Update();

	//Update the sprites
	_pGame->UpdateSprites();

	//Update the saucer to help it dodge the asteroids
	UpdateSaucer();

	//Obtain a device context for repainting the game
	HWND	hWindow = _pGame->GetWindow();
	HDC		hDC = GetDC(hWindow);

	//Paint the game to the offscreen device context
	GamePaint(_hOffscreenDC);

	//Blit the offscreen bitmap to the game screen
	BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
		_hOffscreenDC, 0, 0, SRCCOPY);

	//Cleanup
	ReleaseDC(hWindow, hDC);
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
  return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
}

//=============================================================
//Functions
//=============================================================
void UpdateSaucer()
{
	//Obtain the saucer's position
	RECT rcSaucer, rcRoid;
	rcSaucer = _pSaucer->GetPosition();

	//Find out which asteroid is closest to the saucer
	int iXCollision = 500, iYCollision = 400, iXYCollision = 900;
	for (int i = 0; i < 3; i++)
	{
		//Get the asteroid position
		rcRoid = _pAsteroids[i]->GetPosition();

		//Calculate the minimum XY collision distance
		int iXCollisionDist = (rcSaucer.left +
			(rcSaucer.right - rcSaucer.left) / 2) -
			(rcRoid.left +
			(rcRoid.right - rcRoid.left) / 2);

		int iYCollisionDist = (rcSaucer.top +
			(rcSaucer.bottom - rcSaucer.top) / 2) -
			(rcRoid.top +
			(rcRoid.bottom - rcRoid.top) / 2);

		if ((abs(iXCollisionDist) < abs(iXCollision)) ||
			(abs(iYCollisionDist) < abs(iYCollision)))
			if ((abs(iXCollisionDist) + abs(iYCollisionDist)) < iXYCollision)
			{
				iXYCollision = abs(iXCollision) + abs(iYCollision);
				iXCollision = iXCollisionDist;
				iYCollision = iYCollisionDist;
			}
	}

	//Move to dodge the asteroids, if necessary
	POINT ptVelocity;
	ptVelocity = _pSaucer->GetVelocity();
	
	if (abs(iXCollision) < 60)
	{
		//Adjust the X Velocity
		if (iXCollision < 0)
			ptVelocity.x = max(ptVelocity.x - 1, -8);
		else
			ptVelocity.x = min(ptVelocity.x + 1, 8);
	}

	if (abs(iYCollision) < 60)
	{
		//Adjust the Y Velocity
		if (iYCollision < 0)
			ptVelocity.y = max(ptVelocity.y - 1, -8);
		else
			ptVelocity.y = min(ptVelocity.y + 1, 8);
	}

	//Update the saucer to the new position
	_pSaucer->SetVelocity(ptVelocity);
}
