#include "Enc.h"

EncMain data;
Configure Config;

void main()
{
	system("mode 180");

	////Set default keys
	data.ResetDefault();

	//Add misc-words
	data.AddWord("Book");
	data.AddWord("Lupus");
	data.AddWord("School");
	data.AddWord("Binarys");
	data.AddWord("Sha182azdf");
	data.AddWord("airplanes");

	std::string dat;
	std::getline(std::cin, dat);
	//data.GenArray(dat, Ultra);
	//Config.SaveConfig();
	Config.LoadConfig();

	////////////////////////////////////////////////////////////////////////////
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	////////////////////////////////////////////////////////////////////////////

	std::string dat2 = data.Encrypt(dat);
	std::cout << std::endl << data.Encrypt(dat);
	std::cout << std::endl << std::endl << data.Decrypt(dat2);
	std::cout << std::endl << "Characters Lost: " << dat.length() - ((data.Decrypt(dat2)).length() - 11);


	////////////////////////////////////////////////////////////////////////////
	end = std::chrono::system_clock::now();
	int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();
	int elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>
		(end - start).count();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::cout << std::endl << "Runtime: Milliseconds: " << elapsed_milliseconds << " Microseconds: " << elapsed_microseconds;
	////////////////////////////////////////////////////////////////////////////


	//Save data//
	std::ofstream myfile;
	myfile.open("data.txt"); myfile << data.Encrypt(dat) << std::endl << data.Decrypt(dat2) << std::endl; myfile.close();
	/////////////

	std::cin.get();

	std::cin.get();
}

std::string EncMain::Encrypt(std::string msg)
{
	std::string Final;

	for (int i = 0; i < msg.length(); i++)
	{
		char trans = msg.at(i);
		std::string Current(1, trans);
		std::transform(Current.begin(), Current.end(), Current.begin(), ::tolower);

		Sleep(2);
		sRandomi();
		
		if (Current == "a")
			Final += GenRandom(globalA);
		if (Current == "b")
			Final += GenRandom(globalB);
		if (Current == "c")
			Final += GenRandom(globalC);
		if (Current == "d")
			Final += GenRandom(globalD);
		if (Current == "e")
			Final += GenRandom(globalE);
		if (Current == "f")
			Final += GenRandom(globalF);
		if (Current == "g")
			Final += GenRandom(globalG);
		if (Current == "h")
			Final += GenRandom(globalH);
		if (Current == "i")
			Final += GenRandom(globalI);
		if (Current == "j")
			Final += GenRandom(globalJ);
		if (Current == "k")
			Final += GenRandom(globalK);
		if (Current == "l")
			Final += GenRandom(globalL);
		if (Current == "m")
			Final += GenRandom(globalM);
		if (Current == "n")
			Final += GenRandom(globalN);
		if (Current == "o")
			Final += GenRandom(globalO);
		if (Current == "p")
			Final += GenRandom(globalP);
		if (Current == "q")
			Final += GenRandom(globalQ);
		if (Current == "r")
			Final += GenRandom(globalR);
		if (Current == "s")
			Final += GenRandom(globalS);
		if (Current == "t")
			Final += GenRandom(globalT);
		if (Current == "u")
			Final += GenRandom(globalU);
		if (Current == "v")
			Final += GenRandom(globalV);
		if (Current == "w")
			Final += GenRandom(globalW);
		if (Current == "x")
			Final += GenRandom(globalX);
		if (Current == "y")
			Final += GenRandom(globalY);
		if (Current == "z")
			Final += GenRandom(globalZ);

		static bool Space = true;
		if (Current == " ")
		{
			Final += Space ? "*" : ".";
			Space = Space ? false : true;
		}

		if (Current == ".")
			Final += "_";
		if (Current == "?")
			Final += ",";
		if (Current == ",")
			Final += "%";
		if (Current == "!")
			Final += "?";
		if (Current == "'")
			Final += "#";
		if (Current == "-")
			Final += "=";
		if (Current == "=")
			Final += "-";
		if (Current == "'")
			Final += "/^/";
	}

	return Final;
}

std::string EncMain::Decrypt(std::string msg)
{
	std::string Final;
	Final += "Decrypted: ";

	static int CurrentCount = 0;
	for (int i = 0; i < msg.length(); i++)
	{
		char trans = msg.at(i);
		std::string Current(1, trans);
		std::transform(Current.begin(), Current.end(), Current.begin(), ::tolower);
		if (Current != "_" && Current != "," && Current != "%" && Current != "*" && Current != "." && Current != "?" && Current != "=" && Current != "-" && Current != "'")
		{
			if (Current == "/")
			{
				if (CurrentCount == globalA)
					Final += "a";
				else if (CurrentCount == globalB)
					Final += "b";
				else if (CurrentCount == globalC)
					Final += "c";
				else if (CurrentCount == globalD)
					Final += "d";
				else if (CurrentCount == globalE)
					Final += "e";
				else if (CurrentCount == globalF)
					Final += "f";
				else if (CurrentCount == globalG)
					Final += "g";
				else if (CurrentCount == globalH)
					Final += "h";
				else if (CurrentCount == globalI)
					Final += "i";
				else if (CurrentCount == globalJ)
					Final += "j";
				else if (CurrentCount == globalK)
					Final += "k";
				else if (CurrentCount == globalL)
					Final += "l";
				else if (CurrentCount == globalM)
					Final += "m";
				else if (CurrentCount == globalN)
					Final += "n";
				else if (CurrentCount == globalO)
					Final += "o";
				else if (CurrentCount == globalP)
					Final += "p";
				else if (CurrentCount == globalQ)
					Final += "q";
				else if (CurrentCount == globalR)
					Final += "r";
				else if (CurrentCount == globalS)
					Final += "s";
				else if (CurrentCount == globalT)
					Final += "t";
				else if (CurrentCount == globalU)
					Final += "u";
				else if (CurrentCount == globalV)
					Final += "v";
				else if (CurrentCount == globalW)
					Final += "w";
				else if (CurrentCount == globalX)
					Final += "x";
				else if (CurrentCount == globalY)
					Final += "y";
				else if (CurrentCount == globalZ)
					Final += "z";

				CurrentCount = 0;
				continue;
			}
			else
			{
				CurrentCount += 1;
			}
		}
		else
		{
			if (Current == "_")
				Final += ".";
			if (Current == "=")
				Final += "-";
			if (Current == "-")
				Final += "=";
			if (Current == ",")
				Final += "?";
			if (Current == "%")
				Final += ",";
			if (Current == "?")
				Final += "!";
			if (Current == "*" || Current == ".")
				Final += " ";
			if (Current == "#")
				Final += "";
		}
	}

	return Final;
}

std::string EncMain::GenRandom(int Count)
{
	std::string alphanum = "ABCDEFGHIJKLMN#OPQRSTUVWXYZabcde fg$hijklmnopqrstuvwxyz293814712321][()";

	std::string Ret;
	
	int UseData = 42;
	for (int i = 0; i < data.WordList.size(); i++)
	{
		if (Count == data.WordList[i].Numbs)
			UseData = i;
	}

	if (UseData == 42)
	{
		while (Count != 0)
		{
			Ret += alphanum[rand() % 71];
			Count--;
		}
	}
	else
	{
		Ret += data.WordList[UseData].Text;
	}

	Ret += "/";
	return Ret;
}

void EncMain::sRandomi()
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	LONG time_ms = (time.wSecond * 1000) + time.wMilliseconds;
	srand(time_ms);
}

void EncMain::AddWord(std::string Word)
{
	WordList.push_back(Words());
	WordList[WordList.size() - 1].Numbs = Word.length();
	WordList[WordList.size() - 1].Text = Word;
}

int& EncMain::intToGlobal(int i)
{
	switch (i)
	{
	case 1: return globalA;
	case 2: return globalB;
	case 3: return globalC;
	case 4: return globalD; 
	case 5: return globalE; 
	case 6: return globalF; 
	case 7: return globalG; 
	case 8: return globalH; 
	case 9: return globalI; 
	case 10: return globalJ; 
	case 11: return globalK; 
	case 12: return globalL; 
	case 13: return globalM; 
	case 14: return globalN; 
	case 15: return globalO; 
	case 16: return globalP; 
	case 17: return globalQ; 
	case 18: return globalR; 
	case 19: return globalS; 
	case 20: return globalT; 
	case 21: return globalU; 
	case 22: return globalV; 
	case 23: return globalW; 
	case 24: return globalX; 
	case 25: return globalY; 
	case 26: return globalZ; 
	}
}

void EncMain::ResetDefault()
{
	globalA = A;
	globalB = B;
	globalC = C;
	globalD = D;
	globalE = E;
	globalF = F;
	globalG = G;
	globalH = H;
	globalI = I;
	globalJ = J;
	globalK = K;
	globalL = L;
	globalM = M;
	globalN = N;
	globalO = O;
	globalP = P;
	globalQ = Q;
	globalR = R;
	globalS = S;
	globalT = T;
	globalU = U;
	globalV = V;
	globalW = W;
	globalX = X;
	globalY = Y;
	globalZ = Z;
}

float EncMain::fSqrt(float n)
{
	float x = n;
	float y = 1;
	float e = 0.01f;
	while (x - y > e)
	{
		x = (x + y) / 2;
		y = n / x;
	}
	return x;
}

void EncMain::GenArray(std::string msg, int Type)
{
	int Total = msg.size();
	int Spaces = 0;

	for (int i = 0; i < Total; i++)
	{
		char trans = msg.at(i);
		std::string Current(1, trans);
		if (Current == " ")
		{
			Spaces++;
		}
	}

	int Baseline;
	int Multiple;

	switch (Type)
	{
	case Normal: Baseline = (Total + Spaces) / 2; Multiple = 4; break;
	case Regulated: Baseline = (Total + Spaces + 5) / 2; Multiple = 4; break;
	case Sqrt: Baseline = fSqrt(Total + Spaces); Multiple = 7; break;
	case DoubleDivide: Baseline = ((Total + Spaces) / 2) / 2; Multiple = 2; break;
	case Ultra: Baseline = ((Total + Spaces) / 2) + 12 - 1 + 7; Multiple = 5; break;
	}

	if (Baseline == 0)
	{
		sRandomi();
		Baseline += rand() % 120 + 4;
	}

	bool usingNumb[27] = { false };
	for (int i = 0; i < msg.length(); i++)
	{
		char trans = msg.at(i);
		std::string Current(1, trans);
		std::transform(Current.begin(), Current.end(), Current.begin(), ::tolower);
		if (Current == "a")
		{
			globalA = 1 + Baseline;
			usingNumb[1] = true;
		}
		else if (Current == "b")
		{
			globalB = 2 + Baseline;
			usingNumb[2] = true;
		}
		else if (Current == "c")
		{
			globalC = 3 + Baseline;
			usingNumb[3] = true;
		}
		else if (Current == "d")
		{
			globalD = 4 + Baseline;
			usingNumb[4] = true;
		}
		else if (Current == "e")
		{
			globalE == 5 + Baseline;
			usingNumb[5] = true;
		}
		else if (Current == "f")
		{
			globalF = 6 + Baseline;
			usingNumb[6] = true;
		}
		else if (Current == "g")
		{
			globalG = 7 + Baseline;
			usingNumb[7] = true;
		}
		else if (Current == "h")
		{
			globalH = 8 + Baseline;
			usingNumb[8] = true;
		}
		else if (Current == "i")
		{
			globalI = 9 + Baseline;
			usingNumb[9] = true;
		}
		else if (Current == "j")
		{
			globalJ = 10 + Baseline;
			usingNumb[10] = true;
		}
		else if (Current == "k")
		{
			globalK = 11 + Baseline;
			usingNumb[11] = true;
		}
		else if (Current == "l")
		{
			globalL = 12 + Baseline;
			usingNumb[12] = true;
		}
		else if (Current == "m")
		{
			globalM = 13 + Baseline;
			usingNumb[13] = true;
		}
		else if (Current == "n")
		{
			globalN = 14 + Baseline;
			usingNumb[14] = true;
		}
		else if (Current == "o")
		{
			globalO = 15 + Baseline;
			usingNumb[15] = true;
		}
		else if (Current == "p")
		{
			globalP = 16 + Baseline;
			usingNumb[16] = true;
		}
		else if (Current == "q")
		{
			globalQ = 17 + Baseline;
			usingNumb[17] = true;
		}
		else if (Current == "r")
		{
			globalR = 18 + Baseline;
			usingNumb[18] = true;
		}
		else if (Current == "s")
		{
			globalS = 19 + Baseline;
			usingNumb[19] = true;
		}
		else if (Current == "t")
		{
			globalT = 20 + Baseline;
			usingNumb[20] = true;
 		}
		else if (Current == "u")
		{
			globalU = 21 + Baseline;
			usingNumb[21] = true;
		}
		else if (Current == "v")
		{
			globalV = 22 + Baseline;
			usingNumb[22] = true;
		}
		else if (Current == "w")
		{
			globalW = 23 + Baseline;
			usingNumb[23] = true;
		}
		else if (Current == "x")
		{
			globalX = 24 + Baseline;
			usingNumb[24] = true;
		}
		else if (Current == "y")
		{
			globalY = 25 + Baseline;
			usingNumb[25] = true;
		}
		else if (Current == "z")
		{
			globalZ = 26 + Baseline;
			usingNumb[26] = true;
		}
	}

	int curMultiple = 20;
	for (int i = 1; i < 27; i++)
	{
		int curMultiple = 30;
		if (!usingNumb[i])
		{
			intToGlobal(i) = Multiple * curMultiple++;
		}
	}




	std::vector<int> TotalNumbers;
	for (int i = 1; i < 27; i++)
	{
		TotalNumbers.push_back(int());
		TotalNumbers[i - 1] = intToGlobal(i);
	}

	for (int i = 1; i < 27; i++)
	{
		int MyNumber = TotalNumbers[i - 1];
		for (int itwo = 1; itwo < 27; itwo++)
		{
			if (TotalNumbers[itwo - 1] == MyNumber)
			{
				if (i != itwo)
				{
					TotalNumbers[i - 1] = Multiple * curMultiple++;
					intToGlobal(i) = TotalNumbers[i - 1];
				}
			}
		}
	}
}

std::string EncMain::GenArrayDecryption()
{
	std::string FArray;
	{
		for (int i = 1; i < 27; i++)
		{
			static int lastNumb = 1;
			int numb = intToGlobal(i);

			if (lastNumb != numb && i != 1)
				FArray += "-";

			lastNumb = numb;

			std::string storage = std::to_string(numb);
			for (int iab = 0; iab < storage.length(); iab++)
			{
				char trans = storage.at(iab);
				std::string Current(1, trans);

				if (Current == "1")
					FArray += "one";
				if (Current == "2")
					FArray += "two";
				if (Current == "3")
					FArray += "three";
				if (Current == "4")
					FArray += "four";
				if (Current == "5")
					FArray += "five";
				if (Current == "6")
					FArray += "six";
				if (Current == "7")
					FArray += "seven";
				if (Current == "8")
					FArray += "eight";
				if (Current == "9")
					FArray += "nine";
			}
		}
	}

	return FArray;
}

void Configure::SaveConfig()
{
	ConfigFileOut.open("Config.drim");
	ConfigFileOut << data.GenArrayDecryption();
	ConfigFileOut.close();
}

void Configure::LoadConfig()
{
	ConfigFileIn.open("Config.drim");
	std::string line;
	std::string datatodec;
	while (getline(ConfigFileIn, line))
	{
		datatodec += line;
	}
	ConfigFileIn.close();

	std::string storage;
	std::vector<std::string> All;
	All.push_back("PLACEHOLDER");

	for (int i = 0; i < datatodec.size(); i++)
	{
		char trans = datatodec.at(i);
		std::string Current(1, trans);

		if (Current == "-")
		{
			replaceAll(storage, "one", "1");
			replaceAll(storage, "two", "2");
			replaceAll(storage, "three", "3");
			replaceAll(storage, "four", "4");
			replaceAll(storage, "five", "5");
			replaceAll(storage, "six", "6");
			replaceAll(storage, "seven", "7");
			replaceAll(storage, "eight", "8");
			replaceAll(storage, "nine", "9");
			//std::cout << storage << std::endl;

			All.push_back(storage);
			storage.clear();
			continue;
		}

		storage += Current;
	}

	for (int i = 1; i < All.size() - 1; i++)
	{
		data.intToGlobal(i) = std::stoi(All[i]);
	}
}