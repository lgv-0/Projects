#pragma once
#include "targetver.h"
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <time.h>
#include <thread>
#include <iostream>
#include <algorithm>
#include <gdiplus.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <atlbase.h>
#include <sstream>
#include <algorithm>
#include <iterator>

#pragma comment(lib,"gdiplus.lib")

struct Cell
{
	int Color;
	POINT Position;
};

struct Vec3D
{
	int A;
	int B;
	int C;
	int Output;
};

enum Mode
{
	WolfRules,
	LangtonAnt,
	GameOfLife
};

enum AntDirection
{
	Up,
	Down,
	Left,
	Right
};