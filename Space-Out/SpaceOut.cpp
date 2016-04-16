//=============================================================
//Space Out Application
//Joe Karr
//Created 5-2-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "SpaceOut.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Space Out"),
		TEXT("Space Out"), IDI_SPACEOUT, IDI_SPACEOUT_SM, 600, 450);
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

	//Create and load the bitmaps
	HDC hDC = GetDC(hWindow);
	_pDesertBitmap = new Bitmap(hDC, IDB_DESERT, _hInstance);

	_pCarBitmap = new Bitmap(hDC, IDB_CAR, _hInstance);
	_pSmCarBitmap = new Bitmap(hDC, IDB_SMCAR, _hInstance);
	_pMissileBitmap = new Bitmap (hDC, IDB_MISSILE, _hInstance);

	_pBlobboBitmap = new Bitmap(hDC, IDB_BLOBBO, _hInstance);
	_pBMissileBitmap = new Bitmap (hDC, IDB_BMISSILE, _hInstance);

	_pJellyBitmap = new Bitmap(hDC, IDB_JELLY, _hInstance);
	_pJMissileBitmap = new Bitmap(hDC, IDB_JMISSILE, _hInstance);

	_pTimmyBitmap = new Bitmap(hDC, IDB_TIMMY, _hInstance);
	_pTMissileBitmap = new Bitmap(hDC, IDB_TMISSILE, _hInstance);

	_pSmExplosionBitmap = new Bitmap(hDC, IDB_SMEXPLOSION, _hInstance);
	_pLgExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, _hInstance);

	_pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);

	_pSplashBitmap = new Bitmap(hDC, IDB_SPLASH, _hInstance);
	
	//Create the starry background
	_pBackground = new StarryBackground(600, 450);

	//Set the splash screen variable
	_bDemo = TRUE;
	NewGame();
}

void GameEnd()
{
	//Close the MIDI player for the background music
	_pGame->CloseMIDIPlayer();

	//Cleanup tyhe offscreen device context and bitmap
	DeleteObject(_hOffscreenBitmap);
	DeleteDC(_hOffscreenDC);

	//Cleanup the bitmaps
	delete _pDesertBitmap;
	delete _pCarBitmap;
	delete _pSmCarBitmap;
	delete _pMissileBitmap;
	delete _pBlobboBitmap;
	delete _pBMissileBitmap;
	delete _pJellyBitmap;
	delete _pJMissileBitmap;
	delete _pTimmyBitmap;
	delete _pTMissileBitmap;
	delete _pSmExplosionBitmap;
	delete _pLgExplosionBitmap;
	delete _pGameOverBitmap;
	delete _pSplashBitmap;

	//cleanup the backgroun
	delete _pBackground;

	//Cleanup the sprites
	_pGame->CleanupSprites();

	//Cleanup the game engine
	delete _pGame;
}

void GameActivate(HWND hWindow)
{
	if (!_bDemo)
		//Resume the background music
		_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
	if (!_bDemo)
		//Pause the background music
		_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
	//Draw the background
	_pBackground->Draw(hDC);

	//Draw the desert bitmap
	_pDesertBitmap->Draw(hDC, 0, 371);

	//Draw the sprites
	_pGame->DrawSprites(hDC);

	if (_bDemo)
	{
		//Draw the splash screen image
		_pSplashBitmap->Draw(hDC, 142, 100, TRUE);
	}
	else
	{
		//Draw the score
		TCHAR szText[64];
		RECT  rect = { 460, 0, 510, 30 };
		wsprintf(szText, "%d", _iScore);
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));
		DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

		//Draw the number of remaining lives (cars)
		for (int i = 0; i < _iNumLives; i++)
			_pSmCarBitmap->Draw(hDC, 520 + (_pSmCarBitmap->GetWidth() * i),
				10, TRUE);

		//Draw the game over message, if necessary
		if (_bGameOver)
			_pGameOverBitmap->Draw(hDC, 190, 149, TRUE);
	}
}

void GameCycle()
{
	if(!_bGameOver)
	{
		if(!_bDemo)
		{
			//Randomly add aliens
			if ((rand() % _iDifficulty) == 0)
				AddAlien();
		}

		//Update the background
		_pBackground->Update();

		//Update the sprites
		_pGame->UpdateSprites();

		//Obtain a device context for repainting the game
		HWND hWindow = _pGame->GetWindow();
		HDC  hDC = GetDC(hWindow);

		//Paint the game to the offscreen device context
		GamePaint(_hOffscreenDC);

		//Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
			_hOffscreenDC, 0, 0, SRCCOPY);

		//Cleanup
		ReleaseDC(hWindow, hDC);
	}
	else if (--_iGameOverDelay == 0)
	{
		//Stop the usic and switch to demo mode
		_pGame->PauseMIDISong();
		_bDemo = TRUE;
		NewGame();
	}
}

void HandleKeys()
{
	if(!_bGameOver && !_bDemo)
	{
		//Move the car based upon the left/right key presses
		POINT ptVelocity = _pCarSprite->GetVelocity();
		
		if (GetAsyncKeyState(VK_LEFT) < 0)
		{
			//Move left
			ptVelocity.x = max((int)ptVelocity.x -1, -4);
			_pCarSprite->SetVelocity(ptVelocity);
		}

		else if (GetAsyncKeyState(VK_RIGHT) < 0)
		{
			//Move right
			ptVelocity.x = min((int)ptVelocity.x + 2, 6);
			_pCarSprite->SetVelocity(ptVelocity);
		}

		//Fire missiles based upon spacebar presses
		if ((++_iFireInputDelay > 6) && GetAsyncKeyState(VK_SPACE) < 0)
		{
			//Create a new missile sprite
			RECT  rcBounds = { 0, 0, 600, 450 };
			RECT  rcPos = _pCarSprite->GetPosition();
			Sprite* pSprite = new Sprite(_pMissileBitmap, rcBounds, BA_DIE);
			pSprite->SetPosition(rcPos.left + 15, 400);
			pSprite->SetVelocity(0, -7);
			_pGame->AddSprite(pSprite);

			//Play the missile (fire) sounds
			PlaySound((LPCSTR)IDW_MISSILE, _hInstance, SND_ASYNC | 
				SND_RESOURCE | SND_NOSTOP);

			//reset the input delay
			_iFireInputDelay = 0;
		}
	}
	
	//Start a new game based upon an Enter (Return) key press
	if (GetAsyncKeyState(VK_RETURN) < 0)
		if (_bDemo)
		{
			//Start a new game without the splash screen
			_bDemo = FALSE;
			NewGame();
		}
		else if (_bGameOver)
		{	
			//Start a new game
			NewGame();
		}
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
	//See if a player missile and an alien have collided
	Bitmap* pHitter = pSpriteHitter->GetBitmap();
	Bitmap* pHittee = pSpriteHittee->GetBitmap();

	if ((pHitter == _pMissileBitmap && (pHittee == _pBlobboBitmap ||
		pHittee == _pJellyBitmap || pHittee == _pTimmyBitmap)) ||
		(pHittee == _pMissileBitmap && (pHitter == _pBlobboBitmap ||
		pHitter == _pJellyBitmap || pHitter == _pTimmyBitmap)))
	{
		//Play the small explosion sound
		PlaySound((LPCSTR)IDW_LGEXPLODE, _hInstance, SND_ASYNC |
			SND_RESOURCE);

		//Kill both sprites
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();

		//Create a larget explosion sprite at the alien's position
		RECT rcBounds = { 0, 0, 600, 450 };
		RECT rcPos;

		if (pHitter == _pMissileBitmap)
			rcPos = pSpriteHittee->GetPosition();
		else
			rcPos = pSpriteHitter->GetPosition();

		Sprite* pSprite = new Sprite (_pLgExplosionBitmap, rcBounds);
		pSprite->SetNumFrames(8, TRUE);
		pSprite->SetPosition(rcPos.left, rcPos.top);
		_pGame->AddSprite(pSprite);

		//Update the score
		_iScore += 25;
		_iDifficulty = max(80 - (_iScore / 20), 20);
	}

	//See if an alien missile has collided with the car
	if ((pHitter == _pCarBitmap && (pHittee == _pBMissileBitmap ||
		pHittee == _pJMissileBitmap || pHittee == _pTMissileBitmap)) ||
		(pHittee == _pCarBitmap && (pHitter == _pBMissileBitmap ||
		pHitter == _pJMissileBitmap || pHitter == _pTMissileBitmap)))

	{
		//Play the large explosion sound
		PlaySound((LPCSTR)IDW_LGEXPLODE, _hInstance, SND_ASYNC |
			SND_RESOURCE);

		//Kill the missile sprite
		if (pHitter == _pCarBitmap)
			pSpriteHittee->Kill();
		else
			pSpriteHitter->Kill();

		//Create a larget explosion sprite at the car's position
		RECT rcBounds = { 0, 0, 600, 480 };
		RECT rcPos;

		if (pHitter == _pCarBitmap)
			rcPos = pSpriteHitter->GetPosition();
		else
			rcPos = pSpriteHittee->GetPosition();
		
		Sprite* pSprite = new Sprite(_pLgExplosionBitmap, rcBounds);
		pSprite->SetNumFrames(8, TRUE);
		pSprite->SetPosition(rcPos.left, rcPos.top);
		_pGame->AddSprite(pSprite);

		//Move the car back to the start
		_pCarSprite->SetPosition(300, 405);

		//See if the game is over
		if (--_iNumLives == 0)
		{
			//Play the game over sound
			PlaySound((LPCSTR)IDW_GAMEOVER, _hInstance, SND_ASYNC |
				SND_RESOURCE);
			_bGameOver = TRUE;
			_iGameOverDelay = 90;
		}
	}

	return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
	//See if an alien missile sprite is dying
	if (pSprite->GetBitmap() == _pBMissileBitmap ||
		pSprite->GetBitmap() == _pJMissileBitmap ||
		pSprite->GetBitmap() == _pTMissileBitmap)
	{
		//Play the small explosion sound
		if(!_bDemo)
			PlaySound((LPCSTR)IDW_SMEXPLODE, _hInstance, SND_ASYNC |
					SND_RESOURCE | SND_NOSTOP);

		//Create a small explosion sprite at the missile's position
		RECT rcBounds = { 0, 0, 600, 450 };
		RECT rcPos = pSprite->GetPosition();
		Sprite* pSprite = new Sprite(_pSmExplosionBitmap, rcBounds);
		pSprite->SetNumFrames(8, TRUE);
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

	//Initialize the game variables
	_iFireInputDelay = 0;
	_iScore = 0;
	_iNumLives = 3;
	_iDifficulty = 80;
	_bGameOver = FALSE;

	if (_bDemo)
	{
		//Add a few aliens to the demo
		for (int i = 0; i < 6; i++)
			AddAlien();
	}
	else
	{
		//Create the car sprite
		RECT rcBounds = { 0, 0, 600, 450 };
		_pCarSprite = new Sprite(_pCarBitmap, rcBounds, BA_WRAP);
		_pCarSprite->SetPosition(300, 405);
		_pGame->AddSprite(_pCarSprite);

		//Play the background music
		_pGame->PlayMIDISong(TEXT("Music.mid"));
	}
}

void AddAlien()
{
	//Create a new random alien sprite
	RECT			rcBounds = { 0, 0, 600, 410 };
	AlienSprite* pSprite;
	switch(rand() % 3)
	{
	case 0:
		//Blobbo
		pSprite = new AlienSprite(_pBlobboBitmap, rcBounds, BA_BOUNCE);
		pSprite->SetNumFrames(8);
		pSprite->SetPosition(((rand() % 2) == 0) ? 0 : 600, rand () % 370);
		pSprite->SetVelocity((rand() % 7) - 2, (rand() % 7) - 2);
		break;
	case 1:
		//Jelly
		pSprite = new AlienSprite(_pJellyBitmap, rcBounds, BA_BOUNCE);
		pSprite->SetNumFrames(8);
		pSprite->SetPosition(rand() % 600, rand() % 370);
		pSprite->SetVelocity((rand() % 5) - 2, (rand() % 5) + 3);
		break;
	case 2:
		//Timmy
		pSprite = new AlienSprite(_pTimmyBitmap, rcBounds, BA_WRAP);
		pSprite->SetNumFrames(8);
		pSprite->SetPosition(rand() % 600, rand() % 370);
		pSprite->SetVelocity((rand() % 7) + 3, 0);
		break;
	}

	//Add the alien sprite
	_pGame->AddSprite(pSprite);
}