//=============================================================
//Brainiac Application
//Joe Karr
//Created 4-25-2014
//Updated 4-25-2014
//=============================================================

//=============================================================
//Include files
//=============================================================
#include "Brainiac2.h"

//=============================================================
//Game Engine Functions
//=============================================================
BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Brainiac"),
		TEXT("Brainiac"), IDI_BRAINIAC, IDI_BRAINIAC_SM, 528, 508);
	if (_pGame == NULL)
		return FALSE;

	//Set the frame rate
	_pGame->SetFrameRate(1);

	//Store the instance handle
	_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	//Seed the random number generator
	srand(GetTickCount());

	//Create and load the tile bitmaps
	HDC hDC = GetDC(hWindow);
	_pTiles[0] = new Bitmap(hDC, IDB_TILEBLANK, _hInstance);
	_pTiles[1] = new Bitmap(hDC, IDB_TILE1, _hInstance);
	_pTiles[2] = new Bitmap(hDC, IDB_TILE2, _hInstance);
	_pTiles[3] = new Bitmap(hDC, IDB_TILE3, _hInstance);
	_pTiles[4] = new Bitmap(hDC, IDB_TILE4, _hInstance);
	_pTiles[5] = new Bitmap(hDC, IDB_TILE5, _hInstance);
	_pTiles[6] = new Bitmap(hDC, IDB_TILE6, _hInstance);
	_pTiles[7] = new Bitmap(hDC, IDB_TILE7, _hInstance);
	_pTiles[8] = new Bitmap(hDC, IDB_TILE8, _hInstance);

	//Clear the tile states and images
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			_bTileStates[i][j] = FALSE;
			_iTiles[i][j] = 0;
		}

	//Initialize the tile images randomly
	for (int i = 0; i < 2; i++)
		for (int j = 1; j < 9; j++)
		{
			int x = rand() % 4;
			int y = rand() % 4;
			while (_iTiles[x][y] != 0)
			{
				x = rand() % 4;
				y = rand() % 4;
			}
			_iTiles[x][y] = j;
		}

	//Initialize the tile selections and match/try count
	_ptTile1.x = _ptTile1.y = -1;
	_ptTile2.x = _ptTile2.y = -1;
	_iMatches = _iTries = 0;
}

void GameEnd()
{
	//Cleanup the tile bitmaps
	for (int i = 0; i < 9; i++)
		delete _pTiles[i];

	//Cleanup the game engine
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
	//Draw the tiles
	int iTileWidth = _pTiles[0]->GetWidth();
	int iTileHeight = _pTiles[0]->GetHeight();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (_bTileStates[i][j] || ((i == _ptTile1.x) && (j == _ptTile1.y)) ||
				((i == _ptTile2.x) && (j == _ptTile2.y)))
				
				_pTiles[_iTiles[i][j]]->Draw(hDC, i * iTileWidth, j * iTileHeight,
					TRUE);
			else
				_pTiles[0]->Draw(hDC, i * iTileWidth, j * iTileHeight, TRUE);
}

void GameCycle()
{
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	//Determine which tile was clicked
	int iTileX = x / _pTiles[0]->GetWidth();
	int iTileY = y / _pTiles[0]->GetHeight();

	//Make sure the tile hasn't already been matched
	if (!_bTileStates[iTileX][iTileY])
	{
		//See if this is the first tile selected
		if (_ptTile1.x == -1)
		{
			//Play a sound for the tile selection
			PlaySound((LPCSTR)IDW_SELECT, _hInstance, SND_ASYNC | SND_RESOURCE);
			
			//Set the first tile selection
			_ptTile1.x = iTileX;
			_ptTile1.y = iTileY;
		}
		else if ((iTileX != _ptTile1.x) || (iTileY != _ptTile1.y))
		{
			if (_ptTile2.x == -1)
			{
				//Play a sound for the tile selection
				PlaySound((LPCSTR)IDW_SELECT, _hInstance, SND_ASYNC | SND_RESOURCE);

				//Increase the number of tries
				_iTries++;

				//Set the second tile selection
				_ptTile2.x = iTileX;
				_ptTile2.y = iTileY;

				//See if it's a match
				if (_iTiles[_ptTile1.x][_ptTile1.y] == _iTiles[_ptTile2.x][_ptTile2.y])
				{
					//Play a sound for the tile match
					PlaySound((LPCSTR)IDW_MATCH, _hInstance, SND_ASYNC | SND_RESOURCE);

					//Set the tile state to indicate the match
					_bTileStates[_ptTile1.x][_ptTile1.y] = TRUE;
					_bTileStates[_ptTile2.x][_ptTile2.y] = TRUE;

					//Clear the tile selections
					_ptTile1.x = _ptTile1.y = _ptTile2.x = _ptTile2.y = -1;

					//Update the match count and check for winner
					if (++_iMatches == 8)
					{
						//Play a victory sound
						PlaySound((LPCSTR)IDW_WIN, _hInstance, SND_ASYNC | SND_RESOURCE);

						//Victory message
						TCHAR szText[64];
						wsprintf(szText, "You won in %d tries.", _iTries);
						MessageBox(_pGame->GetWindow(), szText, TEXT("Brainiac"), MB_OK);
					}
				}
				else
					//Play a sound for the tile mismatch
					PlaySound((LPCSTR)IDW_MISMATCH, _hInstance, SND_ASYNC | SND_RESOURCE);
			}
			else
			{
				//Clear the tile selections
				_ptTile1.x = _ptTile1.y = _ptTile2.x = _ptTile2.y = -1;
			}
		}
		
		//Force a repaint to update the tile
		InvalidateRect(_pGame->GetWindow(), NULL, FALSE);
	}
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