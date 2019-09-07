#include "Data.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <Windows.h>
#include <ctime>
#pragma warning(disable:4996)

enum EncLevels
{
	Normal,
	Regulated,
	Sqrt,
	DoubleDivide,
	Ultra
};

class EncMain
{
public:
	void ResetDefault();
	void AddWord(std::string Word);
	void GenArray(std::string msg, int Type);

	std::string GenArrayDecryption();
	std::string Encrypt(std::string msg);
	std::string Decrypt(std::string msg);

	struct Words
	{
		int Numbs;
		std::string Text;
	};

	std::vector<Words> WordList;
	std::vector<Words> test;

	int& intToGlobal(int i);

private:
	std::string GenRandom(int Count);
	void sRandomi();
	float fSqrt(float n);
};

class Configure
{
public:
	std::ofstream ConfigFileOut;
	std::ifstream ConfigFileIn;

	void SaveConfig();
	void LoadConfig();

	void replaceAll(std::string& str, const std::string& from, const std::string& to) {
		if (from.empty())
			return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}
};