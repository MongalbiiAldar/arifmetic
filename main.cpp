#include <stdio.h>
#include <iostream>
#include <string.h>
#include "bigNumber.h"
using namespace std;

void printUsage();
bool checkArguments(int argc, char* argv[]);
bool getFromFiles(char* fileA, char* fileB, char* fileMod, char operation, bool binary, bigNumber& A, bigNumber& B, bigNumber& modulus);
bool process(bigNumber A, bigNumber B, bigNumber modulus, char operation, bigNumber& res);
bool saveRes(char* fileRes, bool binary, bigNumber res);


int main(int argc, char* argv[])
{
	if (!checkArguments(argc, argv))
		return -1;

	char* fileA = argv[1];
	char operation = argv[2][0];
	char* fileB = argv[3];
	char* fileRes = argv[4];
	bool binary = false;
	char* fileMod = NULL;

	if (argc == 6)
	{
		if (!strcmp(argv[5], "-b"))
			binary = true;
		else
			fileMod = argv[5];
	}

	if (argc == 7)
	{
		binary = true;
		fileMod = argv[6];
	}

	bigNumber A, B, modulus;
	if (!getFromFiles(fileA, fileB, fileMod, operation, binary, A, B, modulus))
		return -1;

	bigNumber res;
	if (!process(A, B, modulus, operation, res))
		return -1;

	if (!saveRes(fileRes, binary, res))
		return -1;

	return 0;
}


void printUsage()
{
	cout << "Usage: " << endl;
	cout << "TCHMK_1 <A file name> <operation> <B file name> <result file name> [-b] [<modulus file name>]" << endl << endl;
	cout << "Parameters:" << endl;
	cout << "  -b for operations with binary files" << endl;
	cout << "  operations:" << endl;
	cout << "    \"+\" - addition" << endl;
	cout << "    \"-\" - subtraction" << endl;
	cout << "    \"*\" - multiplication" << endl;
	cout << "    \"/\" - division" << endl;
	cout << "    \"%\" - taking the remainder" << endl;
	cout << "    \"^\" - involution" << endl;
}

bool checkArguments(int argc, char* argv[])
{
	if (argc > 7)
	{
		cout << "Too many arguments passed." << endl;
		printUsage();
		return false;
	}

	if (argc < 5)
	{
		cout << "Too few arguments passed." << endl;
		printUsage();
		return false;
	}

	if (strlen(argv[2]) > 1)
	{
		cout << "Wrong operation." << endl;
		printUsage();
		return false;
	}


	return true;
}

bool getFromFiles(char* fileA, char* fileB, char* fileMod, char operation, bool binary, bigNumber& A, bigNumber& B, bigNumber& modulus)
{
	if (binary)
	{
		if (!A.getNumberFromBinFile(fileA))
		{
			cout << "Can't get number from " << fileA << endl;
			return false;
		}
		if (!B.getNumberFromBinFile(fileB))
		{
			cout << "Can't get number from " << fileB << endl;
			return false;
		}
		if (fileMod)
		{
			if (!modulus.getNumberFromBinFile(fileMod))
			{
				cout << "Can't get number from " << fileMod << endl;
				return false;
			}
		}
	}
	else
	{
		if (!A.getNumberFromTextFile(fileA))
		{
			cout << "Can't get number from " << fileA << endl;
			return false;
		}
		if (!B.getNumberFromTextFile(fileB))
		{
			cout << "Can't get number from " << fileB << endl;
			return false;
		}
		if (fileMod)
		{
			if (!modulus.getNumberFromTextFile(fileMod))
			{
				cout << "Can't get number from " << fileMod << endl;
				return false;
			}
		}
	}

	return true;
}

bool process(bigNumber A, bigNumber B, bigNumber modulus, char operation, bigNumber& res)
{
	if (modulus < (long long int)0)
	{
		cout << "Negative modulus!" << endl;
		return false;
	}

	if (operation == '^')
	{
		res = Pow(A, B, modulus);
		return true;
	}

	if (modulus > (long long int)0)
	{
		A = A % modulus;
		B = B % modulus;
	}

	switch (operation)
	{
	case '+':
		res = A + B;
		break;

	case '-':
		res = A - B;
		break;

	case '*':
		res = A * B;
		break;

	case '/':
		res = A / B;  
		break;

	case '%':
		res = A % B;
		break;
	
	default:
		cout << "Wrong operation." << endl;
		printUsage();
		return false;
	}

	if (modulus > (long long int)0)
	{
		res = res % modulus;
		while (res < (long long int)0)
			res = res + modulus;
	}
	return true;
}

bool saveRes(char* fileRes, bool binary, bigNumber res)
{
	if (binary)
	{
		if (!res.saveNumberToBinFile(fileRes))
		{
			cout << "Can't save result to " << fileRes << endl;
			return false;
		}
	}
	else
	{
		if (!res.saveNumberToTextFile(fileRes))
		{
			cout << "Can't save result to " << fileRes << endl;
			return false;
		}
	}
	return true;
}
