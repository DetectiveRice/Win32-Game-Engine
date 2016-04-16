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

	//Create the starry background
	_pBackground = new StarryBackground(500, 400);

	//Create the asteroid sprites
	RECT	rcBounds = { 0, 0, 500, 400 };
	Sprite* pSprite;

	pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
	pSprite->SetNumFrames(14);
	pSprite->SetFrameDelay(1);
	pSprite->SetPosition(250, 200);
	pSprite->SetVelocity(-3, 1);
	_pGame->AddSprite(pSprite);

	pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
	pSprite->SetNumFrames(14);
	pSprite->SetFrameDelay(2);
	pSprite->SetPosition(250, 200);
	pSprite->SetVelocity(3, -2);
	_pGame->AddSprite(pSprite);

	pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
	pSprite->SetNumFrames(14);
	pSprite->SetFrameDelay(3);
	pSprite->SetPosition(250, 200);
	pSprite->SetVelocity(-2, -4);
	_pGame->AddSprite(pSprite);
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
