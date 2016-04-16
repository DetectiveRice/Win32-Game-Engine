//=============================================================
//Background Object
//Joe Karr
//Create 5-1-2014
//Sams Teach Yourself Game Programming in 24 Hours
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>
#include "Bitmap.h"

//=============================================================
//Background Class
//=============================================================
class Background
{
protected:
	//Member Variables
	int			m_iWidth, m_iHeight;
	COLORREF	m_crColor;
	Bitmap*		m_pBitmap;

public:
	//Constuctors and Destructor
		Background(int iWidth, int iHeight, COLORREF crCOlor);
		Background(Bitmap* pBitmap);
	virtual ~Background();

	//General Methods
	virtual void Update();
	virtual void Draw(HDC hDC);

	//Accessor Methods
	int GetWidth()	{ return m_iWidth; };
	int GetHeight() { return m_iHeight; };
};

//=============================================================
//Starry Background Class
//=============================================================
class StarryBackground : Background
{
protected:
	//Member Variables
	int			m_iNumStars;
	int			m_iTwinkleDelay;
	POINT		m_ptStars[100];
	COLORREF	m_crStarColors[100];

public:
	//Constructor and Destructor
	StarryBackground(int iWidth, int iHeight, int iNumStart = 100,
		int iTwinkleDelay = 50);
	
	virtual ~StarryBackground();

	//General Methods
	virtual void Update();
	virtual void Draw(HDC hDC);
};