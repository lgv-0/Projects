#include "stdafx.h"

/*
Bitmap generation not made by me

Everything else is.

Contains a short-writeup of conway's game of life,
Langton's Ant,
and Wolfram's elementary cellular automata

Will live generate and print on screen results, save to a bitmap, and accept
input for size changes and randomized placements in langton's ant.
*/


// Size of map
int XSize = 1920;
int YSize = 1080;
int LessThan;
int RuleUse = 0;
int Mode = WolfRules;
POINT AntPosition;
AntDirection AntDir = Up;

#pragma region Vector-Defines
Cell CellsTotal[2070601];
std::vector <Vec3D> Ruling;
Cell Container[2070601];
int SizedRet = 0;
#pragma endregion

#pragma region FuncDefines
void SetupWolfRules();
void DetermineFirstPlacements();
void UpdateWolfRules();
void UpdateGameOfLife();
void UpdateAnt();
int GetIDperPosition(POINT position);
std::string DecToPercent(float inc);
#pragma endregion

#pragma region EncoderCLSID
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	using namespace Gdiplus;
	UINT  num = 0;
	UINT  size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return 0;
}
#pragma endregion

#pragma region Bitmaps

#pragma region CreateBitmap
HBITMAP CreateSolidBitmap(HDC hdc, int width, int height, COLORREF cref)
{
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcMem, width, height);
	HBRUSH hbrushFill = CreateSolidBrush(cref);

	HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);
	HBRUSH  hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrushFill);

	Rectangle(hdcMem, 0, 0, width, height);
	for (unsigned int i = 0; i < SizedRet; i++)
	{
		if (CellsTotal[i].Color == 0)
			SetPixelV(hdcMem, CellsTotal[i].Position.x, (Mode != LangtonAnt) ? Container[i].Position.y : CellsTotal[i].Position.y, RGB(255, 255, 255));
		else
			SetPixelV(hdcMem, CellsTotal[i].Position.x, (Mode != LangtonAnt) ? Container[i].Position.y : CellsTotal[i].Position.y, RGB(0, 0, 0));
	}

	SelectObject(hdcMem, hbmpOld);
	SelectObject(hdcMem, hbrushOld);

	DeleteObject(hbrushFill);
	DeleteDC(hdcMem);

	SetBitmapDimensionEx(hbmp, width, height, NULL);

	return hbmp;
}
#pragma endregion

#pragma region DisplayBitmap
void DisplayBitmap(HDC hdc, int x, int y, HBITMAP hbmp)
{
	SIZE sizeBitmap = { 0, 0 };
	GetBitmapDimensionEx(hbmp, &sizeBitmap);

	HDC hdcMem = CreateCompatibleDC(hdc);

	HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);

	BitBlt(hdc, x, y, sizeBitmap.cx, sizeBitmap.cy, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmpOld);

	DeleteDC(hdcMem);
}
#pragma endregion

#pragma region Create_InfoStruct
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		return NULL;

	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	pbmi->bmiHeader.biCompression = BI_RGB;

	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;

	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}
#pragma endregion

#pragma region CreateBMPFile
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;
	BITMAPFILEHEADER hdr;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTotal;
	DWORD cb;
	BYTE *hp;
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
		return;

	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
		return;

	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hf == INVALID_HANDLE_VALUE)
		return;

	hdr.bfType = 0x4d42;

	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		return;
	}

	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		return;

	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		return;

	if (!CloseHandle(hf))
		return;

	GlobalFree((HGLOBAL)lpBits);
}
#pragma endregion

#pragma region JPGtoBMP
void JPGtoBMP(LPCWSTR jpgname, LPCWSTR outputname)
{
	Gdiplus::GdiplusStartupInput startupInput;
	ULONG_PTR token;
	Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
	Gdiplus::Image testImg(jpgname, false);
	CLSID clsid;
	int ret = -1;
	if (-1 != GetEncoderClsid(L"image/bmp", &clsid))
	{
		ret = testImg.Save(outputname, &clsid);
	}
	Gdiplus::GdiplusShutdown(token);
}
#pragma endregion

#pragma region RunBitmap
void RunBitmap()
{
	HDC map = GetDC(NULL);
	HBITMAP sol = CreateSolidBitmap(map, XSize, YSize, RGB(255, 0, 0));
	PBITMAPINFO data = CreateBitmapInfoStruct(GetConsoleWindow(), sol);
	DisplayBitmap(map, 200, 200, sol);
}
#pragma endregion

#pragma endregion

#pragma region MakeMap
void MakeMap()
{
	for (int iY = 1; iY < YSize; iY++)
	{
		for (int iX = 1; iX < XSize; iX++)
		{
			CellsTotal[SizedRet].Color = 0;
			CellsTotal[SizedRet].Position.x = iX;
			CellsTotal[SizedRet++].Position.y = iY;
		}
	}

	LessThan = XSize - 2;
}
#pragma endregion

#pragma region Main
std::string ColorA(int reprint)
{
	return (reprint ? "Black" : "White");
}

int main()
{
	std::cout << "Size X: ";
	std::cin >> XSize;
	std::cout << "\nSize Y: ";
	std::cin >> YSize;

	MakeMap();

	std::cout << "\n0: WolfRules\n1: Langton's Ant\n2: Conway's Game of Life\nMode: ";
	std::cin >> Mode;

	if (Mode == WolfRules)
	{
		std::cout << "\n    Input : Rule\n	0 : 30\n	1 : 73\n	2 : 90\n	3 : 94\n	4 : 110\n	5 : 131\n	6 : 135\n	7 : 139\n	8 : 143\n	9 : 182\n	10 : 250\n";
		std::cin >> RuleUse;
		SetupWolfRules();
	}

	if (Mode == WolfRules)
	{
		std::cout << "Ready to process CA | RULESET: " << std::endl;
		for each (Vec3D s in Ruling)
			std::cout << std::endl << ("Left: " + ColorA(s.A) + " | Right: " + ColorA(s.B) + " | Up: " + ColorA(s.C) + " -> Color Me: " + ColorA(s.Output)).c_str();
		std::cout << std::endl;
	}

	if (Mode == LangtonAnt)
	{
		std::cout << "\nRandomized Interference Count: ";
		int r = 0;
		std::cin >> r;

		for (int i = 0; i < r; i++)
			CellsTotal[rand() % SizedRet + 0].Color = 1;
	}

	DetermineFirstPlacements();

	if (Mode == WolfRules)
		UpdateWolfRules();
	else if (Mode == GameOfLife)
		UpdateGameOfLife();
	else if (Mode == LangtonAnt)
		UpdateAnt();

	//Printing
	for (;;)
	{
		std::cout << "\n --DRAW--";
		std::cin.get();

		HDC map = GetDC(NULL);
		HBITMAP sol = CreateSolidBitmap(map, XSize, YSize, RGB(255, 0, 0));
		PBITMAPINFO data = CreateBitmapInfoStruct(GetConsoleWindow(), sol);
		CreateBMPFile(GetConsoleWindow(), L"Pic.bmp", data, sol, map);
		DisplayBitmap(map, 0, 0, sol);

		std::cout << "\n\n";
		Sleep(500);
	}

	std::cin.get();
	return 0;
}
#pragma endregion

#pragma region Update
void UpdateWolfRules()
{
	std::copy(std::begin(CellsTotal), std::end(CellsTotal), std::begin(Container));

	int PumpCount = 0;

	for (int i = 0; i < SizedRet; i++)
	{
		if (i > 0 && PumpCount++ > 4500)
		{
			std::cout << i << " / " << SizedRet << " @ " << DecToPercent((float)i / (float)SizedRet) << "\n";
			PumpCount = 0;
		}

		if (Container[i].Position.x > 2 && Container[i].Position.y > 2/* && Container[i].Position.x < LessThan && Container[i].Position.y < LessThan*/)
		{
			int neighborhood[4];

			POINT leftOne;
			leftOne.x = Container[i].Position.x - 1;
			leftOne.y = Container[i].Position.y - 1;

			POINT rightOne;
			rightOne.x = Container[i].Position.x + 1;
			rightOne.y = Container[i].Position.y - 1;

			POINT upOne;
			upOne.x = Container[i].Position.x;
			upOne.y = Container[i].Position.y - 1;

			neighborhood[0] = GetIDperPosition(leftOne);
			neighborhood[1] = i;
			neighborhood[2] = GetIDperPosition(rightOne);
			neighborhood[3] = GetIDperPosition(upOne);

			{
				int ARRAY[3];
				ARRAY[0] = Container[neighborhood[0]].Color;
				ARRAY[1] = Container[neighborhood[3]].Color;
				ARRAY[2] = Container[neighborhood[2]].Color;

				for (unsigned int i = 0; i < Ruling.size(); i++)
					if (Ruling[i].A == ARRAY[0])
						if (Ruling[i].B == ARRAY[1])
							if (Ruling[i].C == ARRAY[2])
							{
								Container[neighborhood[1]].Color = Ruling[i].Output;
								break;
							}
			}
		}
	}

	std::copy(std::begin(Container), std::end(Container), std::begin(CellsTotal));
}

void UpdateGameOfLife()
{
	for (int f = 0; f < 10000; f++)
	{
		//Copy array to container
		std::copy(std::begin(CellsTotal), std::end(CellsTotal), std::begin(Container));

		//Loop container
		for (int i = 0; i < SizedRet; i++)
		{
			if (Container[i].Position.x > 2 && Container[i].Position.y > 2 &&
				Container[i].Position.x < LessThan && Container[i].Position.y < LessThan)
			{
				bool IsAlive = (CellsTotal[i].Color == 1);

				int AliveNeighbors = 0;

				if (CellsTotal[i - 1].Color == 1)
					AliveNeighbors++;
				if (CellsTotal[i + 1].Color == 1)
					AliveNeighbors++;

				//Get ID of neighbor up & down
				POINT Up, Down;
				Up.x = CellsTotal[i].Position.x;
				Down.x = CellsTotal[i].Position.x;


				Up.y = CellsTotal[i].Position.y - 1;
				Down.y = CellsTotal[i].Position.y + 1;

				int NeighborUp = GetIDperPosition(Up);
				int NeighborDown = GetIDperPosition(Down);

				//Check for remaining neighbors
				if (CellsTotal[NeighborUp].Color == 1)
					AliveNeighbors++;
				if (CellsTotal[NeighborUp - 1].Color == 1)
					AliveNeighbors++;
				if (CellsTotal[NeighborUp + 1].Color == 1)
					AliveNeighbors++;
				if (CellsTotal[NeighborDown].Color == 1)
					AliveNeighbors++;
				if (CellsTotal[NeighborDown - 1].Color == 1)
					AliveNeighbors++;
				if (CellsTotal[NeighborDown + 1].Color == 1)
					AliveNeighbors++;

				//Rule decisions
				if (IsAlive)
				{
					if (AliveNeighbors < 2 || AliveNeighbors > 3)
						Container[i].Color = 0;
				}
				else if (AliveNeighbors == 3)
					Container[i].Color = 1;
			}
		}

		std::cout << f << std::endl;

		//Print stats
		RunBitmap();

		//Replace array with container
		std::copy(std::begin(Container), std::end(Container), std::begin(CellsTotal));
	}
}

void dirinv(AntDirection& d)
{
	d = (d == Up) ? Down : (d == Down) ? Up : (d == Left) ? Right : Left;
}

void AntForward()
{
	stbk:

	//Move ant forward
	if (AntDir == Up)
	{
		if (AntPosition.y - 1 < 0)
		{
			dirinv(AntDir);
			goto stbk;
		}

		AntPosition.y -= 1;
	}
	else if (AntDir == Down)
	{
		if (AntPosition.y + 1 > YSize)
		{
			dirinv(AntDir);
			goto stbk;
		}

		AntPosition.y += 1;
	}
	else if (AntDir == Left)
	{
		if (AntPosition.x - 1 < 0)
		{
			dirinv(AntDir);
			goto stbk;
		}

		AntPosition.x -= 1;
	}
	else
	{
		if (AntPosition.y + 1 > XSize)
		{
			dirinv(AntDir);
			goto stbk;
		}

		AntPosition.x += 1;
	}
}

void UpdateAnt()
{
	int Pump = 0;
	for (int i = 0; i < 150000; i++)
	{
		//Get Cell ID of our ant's square
		int AntCellID = GetIDperPosition(AntPosition);

		if (CellsTotal[AntCellID].Color == 0)
			//Turn left
			AntDir = (AntDir == Up ? Left : (AntDir == Left) ? Down : (AntDir == Down) ? Right : Up);
		else
			//Turn right
			AntDir = (AntDir == Up ? Right : (AntDir == Right) ? Down : (AntDir == Down) ? Left : Up);

		//Move ant forward
		AntForward();

		//Invert color of square passed
		CellsTotal[AntCellID].Color = (CellsTotal[AntCellID].Color == 1 ? 0 : 1);

		if (Pump++ > 5)
		{
			HANDLE hOut;
			COORD Position;
			hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			Position.X = 0;
			Position.Y = 0;
			SetConsoleCursorPosition(hOut, Position);

			std::cout << "\nPasses: " << i << "\nAnt is at: (" << AntPosition.x << ", " << AntPosition.y << ")";
			RunBitmap();
			Pump = 0;
		}
	}
}
#pragma endregion

#pragma region IDperPos
int GetIDperPosition(POINT position)
{
	int Adder = 1;
	for (unsigned int idf = 0; idf < SizedRet; idf += Adder)
	{
		if (CellsTotal[idf].Position.y == position.y)
		{
			Adder = 1;
			if (CellsTotal[idf].Position.x == position.x)
				return idf;
		}
		else
			Adder = XSize - 1;
	}
	return 7;
}
#pragma endregion

#pragma region WolfSetup
void insertRule(int first, int second, int third, int output)
{
	Ruling.push_back(Vec3D());
	Ruling[Ruling.size() - 1].A = first;
	Ruling[Ruling.size() - 1].B = second;
	Ruling[Ruling.size() - 1].C = third;
	Ruling[Ruling.size() - 1].Output = output;
}

void SetupWolfRules()
{
		//Rule 30
		if (RuleUse == 0)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 0); break;
				case 1: insertRule(1, 1, 0, 0); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 1); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 1); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 0); break;
				}
			}

		//Rule 73
		if (RuleUse == 1)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 0); break;
				case 1: insertRule(1, 1, 0, 1); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 0); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 0); break;
				case 6: insertRule(0, 0, 1, 0); break;
				case 7: insertRule(0, 0, 0, 1); break;
				}
			}

		//Rule 90
		if (RuleUse == 2)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 0); break;
				case 1: insertRule(1, 1, 0, 1); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 1); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 0); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 0); break;
				}
			}

		//Rule 94
		if (RuleUse == 3)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 0); break;
				case 1: insertRule(1, 1, 0, 1); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 1); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 1); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 0); break;
				}
			}

		//Rule 110
		if (RuleUse == 4)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 0); break;
				case 1: insertRule(1, 1, 0, 1); break;
				case 2: insertRule(1, 0, 1, 1); break;
				case 3: insertRule(1, 0, 0, 0); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 1); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 0); break;
				}
			}

		//Rule 131
		if (RuleUse == 5)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 1); break;
				case 1: insertRule(1, 1, 0, 0); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 0); break;
				case 4: insertRule(0, 1, 1, 0); break;
				case 5: insertRule(0, 1, 0, 0); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 1); break;
				}
			}

		//Rule 135
		if (RuleUse == 6)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 1); break;
				case 1: insertRule(1, 1, 0, 0); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 0); break;
				case 4: insertRule(0, 1, 1, 0); break;
				case 5: insertRule(0, 1, 0, 1); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 1); break;
				}
			}

		//Rule 139
		if (RuleUse == 7)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 1); break;
				case 1: insertRule(1, 1, 0, 0); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 0); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 0); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 1); break;
				}
			}

		//Rule 143
		if (RuleUse == 8)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 1); break;
				case 1: insertRule(1, 1, 0, 0); break;
				case 2: insertRule(1, 0, 1, 0); break;
				case 3: insertRule(1, 0, 0, 0); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 1); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 1); break;
				}
			}

		//Rule 182
		if (RuleUse == 9)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 1); break;
				case 1: insertRule(1, 1, 0, 0); break;
				case 2: insertRule(1, 0, 1, 1); break;
				case 3: insertRule(1, 0, 0, 1); break;
				case 4: insertRule(0, 1, 1, 0); break;
				case 5: insertRule(0, 1, 0, 1); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 0); break;
				}
			}

		//Rule 250
		if (RuleUse == 10)
			for (int i = 0; i < 8; i++)
			{
				switch (i)
				{
				case 0: insertRule(1, 1, 1, 1); break;
				case 1: insertRule(1, 1, 0, 1); break;
				case 2: insertRule(1, 0, 1, 1); break;
				case 3: insertRule(1, 0, 0, 1); break;
				case 4: insertRule(0, 1, 1, 1); break;
				case 5: insertRule(0, 1, 0, 0); break;
				case 6: insertRule(0, 0, 1, 1); break;
				case 7: insertRule(0, 0, 0, 0); break;
				}
			}
}
#pragma endregion

#pragma region ToPercent
std::string DecToPercent(float inc)
{
	std::string Original = std::to_string(inc);
	std::string Containment;

	int SinceDecLast = -1;
	for (int i = 0; i < Original.length(); i++)
	{
		if (SinceDecLast > -1)
		{
			if (SinceDecLast++ == 2)
				Containment += '.';

			Containment += Original.at(i);

			continue;
		}

		if (Original.at(i) == '.')
			SinceDecLast = 0;
		else
			Containment += Original.at(i);
	}

	return Containment + "%";
}
#pragma endregion

#pragma region DetermineFirstColors
void DetermineFirstPlacements()
{
	if (Mode == WolfRules)
	{
		POINT pos;
		pos.x = XSize / 2;
		pos.y = 2;
		CellsTotal[GetIDperPosition(pos)].Color = 1;
	}
	else if (Mode == LangtonAnt)
	{
		AntPosition.x = XSize / 2;
		AntPosition.y = YSize / 2;
	}
	else if (Mode == GameOfLife)
	{
		POINT Halfway;
		Halfway.x = XSize / 2;
		Halfway.y = YSize / 2;

		//Glider Gun

		//                         *
		//                       * *
		//            **       **            **
		//           *   *     **            **
		//**        *     *    **
		//**        *   * **     * *
		//          *     *        *
		//           *   *
		//            **

		//Draw square one
		POINT SquareOne[4];
		SquareOne[0].x = Halfway.x - 20;
		SquareOne[0].y = Halfway.y;

		SquareOne[1].x = SquareOne[0].x + 1;
		SquareOne[1].y = Halfway.y;

		SquareOne[2].x = SquareOne[0].x;
		SquareOne[2].y = Halfway.y + 1;

		SquareOne[3].x = SquareOne[1].x;
		SquareOne[3].y = SquareOne[2].y;

		CellsTotal[GetIDperPosition(SquareOne[0])].Color = 1;
		CellsTotal[GetIDperPosition(SquareOne[1])].Color = 1;
		CellsTotal[GetIDperPosition(SquareOne[2])].Color = 1;
		CellsTotal[GetIDperPosition(SquareOne[3])].Color = 1;

		//Draw Circular Piece
		POINT CircLarge[16];
		CircLarge[0].x = SquareOne[1].x + 9;
		CircLarge[0].y = SquareOne[1].y;

		CircLarge[1].x = CircLarge[0].x;
		CircLarge[1].y = CircLarge[0].y + 1;

		CircLarge[2].x = CircLarge[0].x;
		CircLarge[2].y = CircLarge[1].y + 1;

		CircLarge[3].x = CircLarge[0].x + 1;
		CircLarge[3].y = CircLarge[2].y + 1;

		CircLarge[4].x = CircLarge[3].x + 1;
		CircLarge[4].y = CircLarge[3].y + 1;

		CircLarge[5].x = CircLarge[4].x + 1;
		CircLarge[5].y = CircLarge[4].y;

		CircLarge[6].x = CircLarge[0].x + 1;
		CircLarge[6].y = CircLarge[0].y - 1;

		CircLarge[7].x = CircLarge[6].x + 1;
		CircLarge[7].y = CircLarge[6].y - 1;

		CircLarge[8].x = CircLarge[7].x + 1;
		CircLarge[8].y = CircLarge[7].y;

		CircLarge[9].x = CircLarge[1].x + 4;
		CircLarge[9].y = CircLarge[1].y;

		CircLarge[10].x = CircLarge[9].x + 2;
		CircLarge[10].y = CircLarge[9].y;

		CircLarge[11].x = CircLarge[10].x;
		CircLarge[11].y = CircLarge[10].y - 1;

		CircLarge[12].x = CircLarge[11].x - 1;
		CircLarge[12].y = CircLarge[11].y - 1;

		CircLarge[13].x = CircLarge[10].x;
		CircLarge[13].y = CircLarge[10].y + 1;

		CircLarge[14].x = CircLarge[13].x - 1;
		CircLarge[14].y = CircLarge[13].y + 1;

		CircLarge[15].x = CircLarge[10].x + 1;
		CircLarge[15].y = CircLarge[10].y;

		for (int i = 0; i < 16; i++)
			CellsTotal[GetIDperPosition(CircLarge[i])].Color = 1;

		//Draw other thing
		POINT NextThing[12];
		NextThing[0].x = CircLarge[15].x + 3;
		NextThing[0].y = CircLarge[15].y - 1;

		NextThing[1].x = NextThing[0].x;
		NextThing[1].y = NextThing[0].y - 1;

		NextThing[2].x = NextThing[0].x;
		NextThing[2].y = NextThing[1].y - 1;

		NextThing[3].x = NextThing[0].x + 1;
		NextThing[3].y = NextThing[2].y;

		NextThing[4].x = NextThing[3].x;
		NextThing[4].y = NextThing[3].y + 1;

		NextThing[5].x = NextThing[3].x;
		NextThing[5].y = NextThing[3].y + 2;

		NextThing[6].x = NextThing[5].x + 1;
		NextThing[6].y = NextThing[5].y + 1;

		NextThing[7].x = NextThing[6].x;
		NextThing[7].y = NextThing[3].y - 1;

		NextThing[8].x = NextThing[7].x + 2;
		NextThing[8].y = NextThing[7].y;

		NextThing[9].x = NextThing[8].x;
		NextThing[9].y = NextThing[8].y - 1;

		NextThing[10].x = NextThing[6].x + 2;
		NextThing[10].y = NextThing[6].y;

		NextThing[11].x = NextThing[10].x;
		NextThing[11].y = NextThing[10].y + 1;

		for (int i = 0; i < 12; i++)
			CellsTotal[GetIDperPosition(NextThing[i])].Color = 1;

		//Draw square two
		POINT SquareTwo[4];
		SquareTwo[0].x = NextThing[3].x + 13;
		SquareTwo[0].y = NextThing[3].y;

		SquareTwo[1].x = SquareTwo[0].x + 1;
		SquareTwo[1].y = SquareTwo[0].y;

		SquareTwo[2].x = SquareTwo[0].x;
		SquareTwo[2].y = SquareTwo[0].y + 1;

		SquareTwo[3].x = SquareTwo[1].x;
		SquareTwo[3].y = SquareTwo[2].y;

		for (int i = 0; i < 4; i++)
			CellsTotal[GetIDperPosition(SquareTwo[i])].Color = 1;

		//Build a glider
		//  *
		//* *
		// **

		//POINT pos1, pos2, pos3, pos4, pos5;
		//
		//pos1.x = Halfway.x - 2;
		//pos1.y = Halfway.y;

		//pos2.x = pos1.x;
		//pos2.y = pos1.y + 1;

		//pos3.x = pos1.x;
		//pos3.y = pos2.y + 1;

		//pos4.x = pos1.x - 1;
		//pos4.y = pos3.y;

		//pos5.x = pos4.x - 1;
		//pos5.y = pos2.y;

		//CellsTotal[GetIDperPosition(pos1)].Color = 1;
		//CellsTotal[GetIDperPosition(pos2)].Color = 1;
		//CellsTotal[GetIDperPosition(pos3)].Color = 1;
		//CellsTotal[GetIDperPosition(pos4)].Color = 1;
		//CellsTotal[GetIDperPosition(pos5)].Color = 1;
	}
}
#pragma endregion