//=============================================================
//Bitmap Object
//Joe Karr
//Created 4-24-2014
//Updated 4-24-2014
//=============================================================

#pragma once

//=============================================================
//Include Files
//=============================================================
#include <Windows.h>

//=============================================================
//Custom Data Types
//=============================================================
struct BITMAPINFO_256
{
	BITMAPINFOHEADER		bmiHeader;
	RGBQUAD					bmiColors[256];
};

//=============================================================
//Bitmap Class
//=============================================================
class Bitmap
{
protected:
	//Member Variables
	HBITMAP		m_hBitmap;
	int			m_iWidth, m_iHeight;

	//Helper Methods
	void Free();

public:
	//Constructors and Destructor
	Bitmap();
	Bitmap(HDC hDC, LPTSTR szFileName);
	Bitmap(HDC hDC, UINT uiRedID, HINSTANCE hInstance);
	Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
	virtual ~Bitmap();

	//General Methods
	BOOL Create(HDC hDC, LPSTR szFileName);
	BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
	BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
	void Draw(HDC hDC, int x, int y);
	int GetWidth() { return m_iWidth; };
	int GetHeight() { return m_iHeight; };
};