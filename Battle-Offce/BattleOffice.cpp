//=============================================================
//Battle Office Application
//Joe Karr
//Created 4-30-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

//=============================================================
//Include Files
//=============================================================
#include "BattleOffice.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Battle Office"),
		TEXT("Battle Office"), IDI_BATTLEOFFICE, IDI_BATTLEOFFICE_SM, 500, 400);
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

	//Create the offscreen device context and  bitmap
	_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
		_pGame->GetWidth(), _pGame->GetHeight());
	SelectObject(_hOffscreenDC, _hOffscreenBitmap);

	//Create and load the bitmaps
	HDC hDC = GetDC(hWindow);
	_pOfficeBitmap = new Bitmap(hDC, IDB_OFFICE, _hInstance);
	_pTargetBitmap = new Bitmap(hDC, IDB_TARGET, _hInstance);
	_pPowBitmap = new Bitmap(hDC, IDB_POW, _hInstance);
	_pGuyBitmaps[0] = new Bitmap(hDC, IDB_GUY1, _hInstance);
	_pGuyBitmaps[1] = new Bitmap(hDC, IDB_GUY2, _hInstance);
	_pGuyBitmaps[2] = new Bitmap(hDC, IDB_GUY3, _hInstance);
	_pGuyBitmaps[3] = new Bitmap(hDC, IDB_GUY4, _hInstance);
	_pGuyBitmaps[4] = new Bitmap(hDC, IDB_GUY5, _hInstance);
	_pSmallGuyBitmap = new Bitmap(hDC, IDB_SMALLGUY, _hInstance);
	_pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);

	//Create the target sprite
	RECT		rcBounds = { 0, 0, 500, 400 };
	_pTargetSprite = new Sprite(_pTargetBitmap, rcBounds, BA_STOP);
	_pTargetSprite->SetZOrder(4);
	_pGame->AddSprite(_pTargetSprite);

	//Create the pow sprite
	_pPowSprite = new Sprite(_pPowBitmap, rcBounds, BA_STOP);
	_pPowSprite->SetZOrder(3);
	_pPowSprite->SetHidden(TRUE);
	_pGame->AddSprite(_pPowSprite);

	//Create the guy sprites
	_pGuySprites[0] = new Sprite(_pGuyBitmaps[0], rcBounds);
	_pGuySprites[0]->SetPosition(92, 175);
	_pGuySprites[0]->SetZOrder(2);
	_pGuySprites[0]->SetHidden(TRUE);
	_pGame->AddSprite(_pGuySprites[0]);

	_pGuySprites[1] = new Sprite(_pGuyBitmaps[1], rcBounds);
	_pGuySprites[1]->SetPosition(301, 184);
	_pGuySprites[1]->SetZOrder(2);
	_pGuySprites[1]->SetHidden(TRUE);
	_pGame->AddSprite(_pGuySprites[1]);

	_pGuySprites[2] = new Sprite(_pGuyBitmaps[2], rcBounds);
	_pGuySprites[2]->SetPosition(394, 61);
	_pGuySprites[2]->SetZOrder(2);
	_pGuySprites[2]->SetHidden(TRUE);
	_pGame->AddSprite(_pGuySprites[2]);

	rcBounds.left = 340;
	_pGuySprites[3] = new Sprite(_pGuyBitmaps[3], rcBounds, BA_WRAP);
	_pGuySprites[3]->SetPosition(500, 10);
	_pGuySprites[3]->SetVelocity(-3, 0);
	_pGuySprites[3]->SetZOrder(1);
	_pGuySprites[3]->SetHidden(TRUE);
	_pGame->AddSprite(_pGuySprites[3]);

	rcBounds.left = 385;
	_pGuySprites[4] = new Sprite(_pGuyBitmaps[4], rcBounds, BA_WRAP);
	_pGuySprites[4]->SetPosition(260, 60);
	_pGuySprites[4]->SetZOrder(1);
	_pGuySprites[4]->SetHidden(TRUE);
	_pGame->AddSprite(_pGuySprites[4]);

	//Initialize the remaining global variables
	_iGuyMasterDelay = 50;
	_iHits = 0;
	_iMisses = 0;
	_bGameOver = FALSE;

	//Play the background music
	_pGame->PlayMIDISong(TEXT("Music.mid"));
}

void GameEnd()
{
	//Close the MIDI player for the background music
	_pGame->CloseMIDIPlayer();

	//Cleanup the offscreen device context and bitmap
	DeleteObject(_hOffscreenBitmap);
	DeleteDC(_hOffscreenDC);

	//Cleanup the bitmaps
	delete _pOfficeBitmap;
	delete _pTargetBitmap;
	delete _pPowBitmap;
	
	for (int i = 0; i < 5; i++)
		delete _pGuyBitmaps[i];
	delete _pSmallGuyBitmap;
	delete _pGameOverBitmap;

	//Cleanup the sprites
	_pGame->CleanupSprites();

	//Cleanup the game engine
	delete _pGame;
}

void GameActivate(HWND hWindow)
{
	//Resume the backgroun music
	_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
	//Pause the background music
	_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
	//Draw the backgroun office
	_pOfficeBitmap->Draw(hDC, 0, 0);

	//Draw the sprites
	_pGame->DrawSprites(hDC);

	//Draw the number of guys who were hit
	TCHAR szText[64];
	RECT rect = {237, 360, 301, 390};
	wsprintf(szText, "%d", _iHits);
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	//Draw the number of guys who were missed (got away)(
	for (int i = 0; i < _iMisses; i++)
		_pSmallGuyBitmap->Draw(hDC, 389 + (_pSmallGuyBitmap->GetWidth() * i), 359, TRUE);

	//Draw the game over message, if necessary
	if (_bGameOver)
		_pGameOverBitmap->Draw(hDC, 120, 110, TRUE);
}

void GameCycle()
{
	if (!_bGameOver)
	{
		//Randomly show and hide the guys
		for (int i = 0; i < 5; i++)
			if (_pGuySprites[i]->IsHidden())
			{
				if (rand() % 60 == 0)
				{
					//Show the guy
					_pGuySprites[i]->SetHidden(FALSE);

					//Start the countdown delay
					if (i == 3)
					{
						//Start the guy running left
						_iGuyDelay[i] = 80;
						_pGuySprites[i]->SetPosition(500,10);
					}
					else if (i == 4)
					{
						//Start the guy running right
						_iGuyDelay[i] = 45;
						_pGuySprites[i]->SetPosition(260, 60);
					}
					else
						//Start the stationary guys
						_iGuyDelay[i] = 20  + (rand() % _iGuyMasterDelay);
				}
			}
			else
			{
				if (--_iGuyDelay[i] == 0)
				{
					//Play a sound for the guy getting away
					PlaySound((LPCSTR)IDW_TAUNT, _hInstance, SND_ASYNC | SND_RESOURCE);

					//Hide the guy
					_pGuySprites[i]->SetHidden(TRUE);

					//Increment the misses
					if (++_iMisses == 5)
					{
						//Play a sound for the game ending
						PlaySound((LPCSTR)IDW_BOO, _hInstance, SND_ASYNC | SND_RESOURCE);

						//End the game
						for (int i = 0; i < 5; i++)
							_pGuySprites[i]->SetHidden(TRUE);
						_bGameOver = TRUE;

						//Pause the background music
						_pGame->PauseMIDISong();
					}
				}
			}

			//Update the Sprites
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
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	//Only check the left mouse button
	if (!_bGameOver && bLeft)
	{
		//Temporarily hide the target and pow sprites
		_pTargetSprite->SetHidden(TRUE);
		_pPowSprite->SetHidden(TRUE);

		//See if a guy sprite was clicked
		Sprite* pSprite;
		if ((pSprite = _pGame->IsPointInSprite(x, y)) != NULL)
		{
			//Play a sound for hitting the guy
			PlaySound((LPCSTR)IDW_WHACK, _hInstance, SND_ASYNC | SND_RESOURCE);

			//Position and show the pow sprite
			_pPowSprite->SetPosition(x - (_pPowSprite->GetWidth() / 2),
				y - (_pPowSprite->GetHeight() / 2));
			_pPowSprite->SetHidden(FALSE);

			//Hide the guy that was clicked
			pSprite->SetHidden(TRUE);

			//Increment the hits and make the game harder, if necessary
			if ((++_iHits % 5) == 0)
				if (--_iGuyMasterDelay == 0)
					_iGuyMasterDelay = 1;
		}

		//Show the target sprite again
		_pTargetSprite->SetHidden(FALSE);
	}
	else if (_bGameOver && !bLeft)
	{
		//Start a new game
		_bGameOver = FALSE;
		_iHits = 0;
		_iMisses = 0;

		//Restart the background music
		_pGame->PlayMIDISong();
	}
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
	//Track the mouse with the target sprite
	_pTargetSprite->SetPosition(x - (_pTargetSprite->GetWidth() / 2),
		y - (_pTargetSprite->GetHeight() / 2));
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	return FALSE;
}