//=============================================================
//Meteor Defense Application
//Joe Karr
//Created 5-1-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "MeteorDefense.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
  // Create the game engine
  _pGame = new GameEngine(hInstance, TEXT("Meteor Defense"),
    TEXT("Meteor Defense"), IDI_METEORDEFENSE, IDI_METEORDEFENSE_SM, 600, 450);
  if (_pGame == NULL)
    return FALSE;

  // Set the frame rate
  _pGame->SetFrameRate(30);

  // Store the instance handle
  _hInstance = hInstance;

  return TRUE;
}

void GameStart(HWND hWindow)
{
  // Seed the random number generator
  srand(GetTickCount());

  // Create the offscreen device context and bitmap
  _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
  _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
    _pGame->GetWidth(), _pGame->GetHeight());
  SelectObject(_hOffscreenDC, _hOffscreenBitmap);

  // Create and load the bitmaps
  HDC hDC = GetDC(hWindow);
  _pGroundBitmap = new Bitmap(hDC, IDB_GROUND, _hInstance);
  _pTargetBitmap = new Bitmap(hDC, IDB_TARGET, _hInstance);
  _pCityBitmap = new Bitmap(hDC, IDB_CITY, _hInstance);
  _pMeteorBitmap = new Bitmap(hDC, IDB_METEOR, _hInstance);
  _pMissileBitmap = new Bitmap(hDC, IDB_MISSILE, _hInstance);
  _pExplosionBitmap = new Bitmap(hDC, IDB_EXPLOSION, _hInstance);
  _pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);

  // Create the starry background
  _pBackground = new StarryBackground(600, 450);

  // Play the background music
  _pGame->PlayMIDISong(TEXT("Music.mid"));

  // Start the game
  NewGame();
}

void GameEnd()
{
  // Close the MIDI player for the background music
  _pGame->CloseMIDIPlayer();

  // Cleanup the offscreen device context and bitmap
  DeleteObject(_hOffscreenBitmap);
  DeleteDC(_hOffscreenDC);  

  // Cleanup the bitmaps
  delete _pGroundBitmap;
  delete _pTargetBitmap;
  delete _pCityBitmap;
  delete _pMeteorBitmap;
  delete _pMissileBitmap;
  delete _pExplosionBitmap;
  delete _pGameOverBitmap;

  // Cleanup the background
  delete _pBackground;

  // Cleanup the sprites
  _pGame->CleanupSprites();

  // Cleanup the game engine
  delete _pGame;
}

void GameActivate(HWND hWindow)
{
  // Resume the background music
  _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
  // Pause the background music
  _pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
	//Draw the background
	_pBackground->Draw(hDC);

	//Draw the ground bitmap
	_pGroundBitmap->Draw(hDC, 0, 398, TRUE);

	//Draw the sprites
	_pGame->DrawSprites(hDC);

	//Draw the score
	TCHAR	 szText[64];
	RECT	 rect = { 275, 0, 325, 50 };
	wsprintf(szText, "%d", _iScore);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	//Draw the game over message, if necessary
	if (_bGameOver)
		_pGameOverBitmap->Draw(hDC, 170, 150, TRUE);
}

void GameCycle()
{
	if (!_bGameOver)
	{
		//Randomly add meteors
		if ((rand() % _iDifficulty) == 0)
			AddMeteor();

		//Update the background
		_pBackground->Update();

		//Update the sprites
		_pGame->UpdateSprites();

		// Obtain a device context for repainting the game
		HWND  hWindow = _pGame->GetWindow();
		HDC   hDC = GetDC(hWindow);

		// Paint the game to the offscreen device context
		GamePaint(_hOffscreenDC);

		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
			_hOffscreenDC, 0, 0, SRCCOPY);

		// Cleanup
		ReleaseDC(hWindow, hDC);
  }
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	if (!_bGameOver && bLeft)
	{
		//Create a new missile sprite and set its position
		RECT		rcBounds = { 0, 0, 600, 450 };
		int			iXPos = (x < 300) ? 144 : 449;
		Sprite*		pSprite = new Sprite(_pMissileBitmap, rcBounds, BA_DIE);
		pSprite->SetPosition(iXPos, 365);

		//Calculate the velocity so that it is aimed at the target
		int iXVel, iYVel = -6;
		y = min(y, 300);
		iXVel = (iYVel * ((iXPos + 8) - x)) / (365 - y);
		pSprite->SetVelocity(iXVel, iYVel);

		//Add the missile sprite
		_pGame->AddSprite(pSprite);

		//Play the fire sound
		PlaySound((LPCSTR)IDW_FIRE, _hInstance, SND_ASYNC |
			SND_RESOURCE | SND_NOSTOP);

		//Update the score
		_iScore = max(--_iScore, 0);
	}
	else if (_bGameOver && !bLeft)
		//Start a new game
		NewGame();
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
  // Track the mouse with the target sprite
  _pTargetSprite->SetPosition(x - (_pTargetSprite->GetWidth() / 2),
    y - (_pTargetSprite->GetHeight() / 2));
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	//See if a missile and a meteor have collided
	if ((pSpriteHitter->GetBitmap() == _pMissileBitmap &&
		pSpriteHittee->GetBitmap() == _pMeteorBitmap) ||
		(pSpriteHitter->GetBitmap() == _pMeteorBitmap &&
		pSpriteHittee->GetBitmap() == _pMissileBitmap))
	{
		//Kill both sprites
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();
		SpriteDying(pSpriteHitter);
		SpriteDying(pSpriteHittee);

		//Update the score
		_iScore += 6;
		_iDifficulty = max(50 - (_iScore / 10), 5);
	}

	//See if a meteor has collided with a city
	if (pSpriteHitter->GetBitmap() == _pMeteorBitmap &&
		pSpriteHittee->GetBitmap() == _pCityBitmap)
	{
		//Play the big explosion sound
		PlaySound((LPCSTR)IDW_BIGEXPLODE, _hInstance, SND_ASYNC |
			SND_RESOURCE);

		//Kill both sprites
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();

		//See if the game is over
		if (--_iNumCities == 0)
			_bGameOver = TRUE;
	}
	
	return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
	//See if a meteor sprite is dying
	if (pSprite->GetBitmap() == _pMeteorBitmap)
	{
		//Play the explosion sound
		PlaySound((LPCSTR)IDW_EXPLODE, _hInstance, SND_ASYNC |
			SND_RESOURCE | SND_NOSTOP);

		//Create an explosion sprite at the meteor's position
		RECT rcBounds = { 0, 0, 600, 450 };
		RECT rcPos = pSprite->GetPosition();
		Sprite* pSprite = new Sprite(_pExplosionBitmap, rcBounds);
		pSprite->SetNumFrames(12, TRUE);
		pSprite->SetPosition(rcPos.left, rcPos.top);
		_pGame->AddSprite(pSprite);
	}
}

//=============================================================
//Functions
//=============================================================
void NewGame()
{
	//Clear the sprites
	_pGame->CleanupSprites();

	// Create the target sprite
  RECT rcBounds = { 0, 0, 600, 450 };
  _pTargetSprite = new Sprite(_pTargetBitmap, rcBounds, BA_STOP);
  _pTargetSprite->SetZOrder(10);
  _pGame->AddSprite(_pTargetSprite);

  // Create the city sprites
  Sprite* pSprite = new Sprite(_pCityBitmap, rcBounds);
  pSprite->SetPosition(2, 370);
  _pGame->AddSprite(pSprite);
  pSprite = new Sprite(_pCityBitmap, rcBounds);
  pSprite->SetPosition(186, 370);
  _pGame->AddSprite(pSprite);
  pSprite = new Sprite(_pCityBitmap, rcBounds);
  pSprite->SetPosition(302, 370);
  _pGame->AddSprite(pSprite);
  pSprite = new Sprite(_pCityBitmap, rcBounds);
  pSprite->SetPosition(490, 370);
  _pGame->AddSprite(pSprite);

  // Initialize the game variables
  _iScore = 0;
  _iNumCities = 4;
  _iDifficulty = 50;
  _bGameOver = FALSE;

  // Play the background music
  _pGame->PlayMIDISong();
}

void AddMeteor()
{
	//Create a new meteor sprite and set its position
	RECT		rcBounds = { 0, 0, 600, 390 };
	int			iXPos = rand() % 600;
	Sprite*		pSprite = new Sprite(_pMeteorBitmap, rcBounds, BA_DIE);
	pSprite->SetNumFrames(14);
	pSprite->SetPosition(iXPos, 0);

	//Calculate the velocity so that it is aimed at one of the cities
	int iXVel, iYVel = (rand() % 4) + 3;
	switch(rand() % 4)
	{
	case 0:
		iXVel = (iYVel * (56 - (iXPos + 50))) / 400;
		break;
	case 1:
		iXVel = (iYVel * (240 - (iXPos + 50))) / 400;
		break;
	case 2:
		iXVel = (iYVel * (360 - (iXPos + 50))) / 400;
		break;
	case 3:
		iXVel = (iYVel * (546 - (iXPos + 50))) / 400;
		break;
	}
	pSprite->SetVelocity(iXVel, iYVel);
	
	//Add the meteor sprite
	_pGame->AddSprite(pSprite);
}