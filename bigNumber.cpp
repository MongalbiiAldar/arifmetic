#include "bigNumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using std::ifstream;
using std::ofstream;

bigNumber::bigNumber()
{
	this->_sign = 0;
	this->_elements.push_back(0);
}

bigNumber::bigNumber(char* text)
{
	if (!text)
		return;

	int strSize = strlen(text);
	int strSign = 0;
	if (text[0] == '-')
	{
		strSize--;
		strSign = 1;
	}

	const char* pStr = text + strSign;
	while (*pStr)
	{
		if (*pStr < '0' || *pStr > '9')
		{
			this->_elements.push_back(0);
			this->_sign = 0;
			return;
		}
		pStr++;
	}

	for (int i = 0; i < (strSize + strSign) / 9; i++)
	{
		pStr -= 9;
		char splStr[10];
		memcpy(splStr, pStr, 9);
		splStr[9] = '\0'; 
		unsigned int digitI = atol(splStr);
		this->_elements.push_back(digitI);
	}

	char ost[10];
	memset(ost, 0, 10);
	memcpy(ost, text + strSign, pStr - text - strSign);
	if (strlen(ost) > 0)
	{
		unsigned int lastDigit = atol(ost);
		this->_elements.push_back(lastDigit);
	}

	this->_sign = strSign;
	this->_DelZeroes();
}

bigNumber::bigNumber(long long int value)
{
	this->_sign = 0;
	if (value < 0)
	{
		this->_sign = 1;
		value = -value;
	}
	do
	{
		this->_elements.push_back(value % BASE);
		value = value / BASE;
	} while (value != 0);
}


char* bigNumber::getString()
{
	char* strBuffer = new char[this->_elements.size() * 9 + 1 + this->_sign]();
	char* pString = strBuffer + this->_elements.size() * 9 + this->_sign; 

	for (int i = 0; i < this->_elements.size(); i++)
	{
		char splStr[10];
		sprintf(splStr, "%09u", this->_elements[i]);

		pString -= 9;
		memcpy(pString, splStr, 9);
	}

	while (*pString == '0' && *(pString + 1))
		pString++;

	if (this->_sign)
	{
		pString--;
		*pString = '-';
	}

	char* string = new char[strlen(pString) + 1]();
	strcpy(string, pString);
	delete[] strBuffer;

	return string;
}

bool bigNumber::getNumberFromTextFile(const char* filename)
{
	ifstream Text_file(filename);
	if (Text_file.fail())
		return false;

	Text_file.seekg(0, std::ios::end);
	int SizeOfFile = Text_file.tellg();
	Text_file.seekg(0, std::ios::beg);

	char* string = new char[SizeOfFile + 1]();
	Text_file >> string;
	Text_file.close();

	*this = bigNumber(string);
	delete[] string;
	return true;
}

bool bigNumber::saveNumberToTextFile(const char* filename)
{
	ofstream Result_file(filename);
	if (Result_file.fail())
		return false;

	char* string = this->getString();
	Result_file << string;
	delete[] string;
	Result_file.close();

	return true;
}

bool bigNumber::saveNumberToBinFile(const char* filename)
{
	ofstream Result_file(filename, std::ios::binary);
	if (Result_file.fail())
		return false;

	bigNumber temp = *this; 
	bigNumber b256 = 256; 
	bigNumber b0 = (long long int)0;

	while (temp != b0)
	{
		bigNumber remainder;
		temp = _dividing(temp, b256, remainder); 
		unsigned char ost = remainder[0];
		Result_file.write((char*)&ost, 1);
	}

	Result_file.close();
	return true;
}

bool bigNumber::getNumberFromBinFile(const char* filename)
{
	ifstream Bin_file(filename, std::ios::binary);

	if (Bin_file.fail())
		return false;

	Bin_file.seekg(0, std::ios::end);
	int SizeOfFile = Bin_file.tellg();
	Bin_file.seekg(0, std::ios::beg);

	unsigned char* fileContent = new unsigned char[SizeOfFile];
	Bin_file.read((char*)fileContent, SizeOfFile);
	Bin_file.close();

	bigNumber res;
	bigNumber b256 = 1;
	for (int i = 0; i < SizeOfFile; i++)
	{
		unsigned int fi = fileContent[i];
		res = res + b256 * fi;
		b256 = b256 * 256;
	}
	*this = res;
	return true;
}

void bigNumber::_DelZeroes()
{
	while (this->_elements.size() > 1 && this->_elements.back() == 0)
		this->_elements.pop_back();

	if (this->_elements.size() == 1 && this->_elements[0] == 0)
		this->_sign = 0;
}

long long int bigNumber::_compare(const bigNumber& B)
{
	int thisSign = 1;
	if (this->_sign == 1)
		thisSign = -1;

	if (this->_sign != B._sign)
		return thisSign;

	if (this->_elements.size() > B._elements.size())
		return thisSign;

	if (this->_elements.size() < B._elements.size())
		return -thisSign;

	int i = this->_elements.size() - 1;

	while (this->_elements[i] == B[i] && i > 0)
	{
		i--;
	}
	return ((long long int) this->_elements[i] - (long long int)B[i])*thisSign;
}

void bigNumber::_shiftLeft(int s)
{
	std::vector<unsigned int> newElements;

	newElements.insert(newElements.end(), s, 0);
	newElements.insert(newElements.end(), this->_elements.begin(), this->_elements.end());

	this->_elements = newElements;
}

bigNumber bigNumber::_sum(const bigNumber& A, const bigNumber& B) const
{
	bigNumber res;
	res._elements.resize(A._elements.size() + 1);

	unsigned int carry = 0;
	for (int i = 0; i < B._elements.size(); i++)
	{
		unsigned int tmp = A[i] + B[i] + carry;
		res[i] = tmp % BASE;
		carry = tmp / BASE;
	}

	for (int i = B._elements.size(); i < A._elements.size(); i++)
	{
		unsigned int tmp = A[i] + carry;
		res[i] = tmp % BASE;
		carry = tmp / BASE;
	}
	res[A._elements.size()] = carry;
	res._sign = A._sign;
	res._DelZeroes();
	return res;
}

bigNumber bigNumber::_sub(const bigNumber& A, const bigNumber& B) const
{
	bigNumber res;
	res._elements.resize(A._elements.size());

	unsigned int carry = 0;
	for (int i = 0; i < B._elements.size(); i++)
	{
		int tmp = A[i] - B[i] - carry;
		carry = 0;
		if (tmp < 0)
		{
			carry = 1;
			tmp += BASE;
		}
		res[i] = tmp;
	}

	for (int i = B._elements.size(); i < A._elements.size(); i++)
	{
		int tmp = A[i] - carry;
		carry = 0;
		if (tmp < 0)
		{
			carry = 1;
			tmp += BASE;
		}
		res[i] = tmp;
	}
	res._sign = A._sign;
	res._DelZeroes();
	return res;
}

bigNumber bigNumber::_dividing(const bigNumber& A, const bigNumber& B, bigNumber &remainder) const
{
	remainder = A;
	remainder._sign = 0;

	bigNumber divider = B;
	divider._sign = 0;

	if (divider == bigNumber((long long int) 0))
	{
		remainder = (long long int) 0;
		return bigNumber((long long int)(0));
	}

	if (remainder < divider)
	{
		remainder = A;
		return bigNumber((long long int) 0);
	}

	if (divider._elements.size() == 1)
	{
		bigNumber res;
		res._elements.resize(A._elements.size());
		unsigned long long int carry = 0;
		for (int i = A._elements.size() - 1; i >= 0; i--)
		{
			unsigned long long int temp = carry;
			temp *= BASE;
			temp += A[i];
			res[i] = temp / divider[0];
			carry = (unsigned long long int)  temp - (unsigned long long int) res[i] * (unsigned long long int) divider[0];
		}
		remainder._sign = (!A._sign != B._sign);
		remainder._elements.clear();
		remainder._elements.push_back(carry);
		remainder._DelZeroes();
		res._sign = (!A._sign != B._sign);
		res._DelZeroes();
		return res;
	}


	bigNumber res;
	res._elements.resize(A._elements.size() - B._elements.size() + 1);

	for (int i = A._elements.size() - B._elements.size() + 1; i; i--)
	{
		long long int qGuessMax = BASE; 
		long long int qGuessMin = 0;
		long long int qGuess = qGuessMax;

		while (qGuessMax - qGuessMin > 1)
		{
			qGuess = (qGuessMax + qGuessMin) / 2;

			bigNumber tmp = divider * qGuess;
			tmp._shiftLeft(i - 1);	
			if (tmp > remainder)
				qGuessMax = qGuess;
			else
				qGuessMin = qGuess;
		}
		bigNumber tmp = divider * qGuessMin;
		tmp._shiftLeft(i - 1); 
		remainder = remainder - tmp;
		res[i - 1] = qGuessMin;
	}

	res._sign = (A._sign != B._sign);
	remainder._sign = (A._sign != B._sign);
	remainder._DelZeroes();
	res._DelZeroes();

	return res;
}

bigNumber Pow(const bigNumber& A, const bigNumber& B, bigNumber& modulus)
{
	if (modulus <= (long long int) 0)
		return A ^ B;

	bigNumber base = A % modulus;
	bigNumber res = 1;

	for (bigNumber i = B; i > (long long int) 0; i = i - 1)
		res = (res * base) % modulus;

	return res;
}


unsigned int & bigNumber::operator[](int i)
{
	return this->_elements[i];
}

unsigned int bigNumber::operator[](int i) const
{
	return this->_elements[i];
}

bigNumber bigNumber::operator+(const bigNumber& right) const
{
	bigNumber A = *this;
	bigNumber B = right;
	A._sign = 0;
	B._sign = 0;
	if (A > B)
	{
		A._sign = this->_sign;
		B._sign = right._sign;
	}
	else
	{
		A = right;
		B = *this;
	}

	if (A._sign == B._sign)
	{
		return _sum(A, B);
	}
	else
	{
		return _sub(A, B);
	}
}

bigNumber bigNumber::operator-() const
{
	bigNumber res = *this;
	res._sign = !res._sign;
	return res;
}

bigNumber bigNumber::operator-(const bigNumber& right) const
{
	return bigNumber(*this + (-right));
}

bigNumber bigNumber::operator*(const bigNumber& B) const
{
	bigNumber A = *this;
	bigNumber res;
	res._elements.resize(A._elements.size() + B._elements.size());
	unsigned int carry = 0;
	for (int i = 0; i < B._elements.size(); i++)
	{
		carry = 0;
		for (int j = 0; j < A._elements.size(); j++)
		{
			unsigned long long int tmp = (unsigned long long int) B[i] * (unsigned long long int) A[j]
				+ carry + (unsigned long long int) res[i + j];
			carry = tmp / BASE;
			res[i + j] = tmp % BASE;
		}
		res[i + A._elements.size()] = carry;
	}

	res._sign = (A._sign != B._sign);
	res._DelZeroes();
	return res;
}

bigNumber bigNumber::operator/(const bigNumber& right) const
{
	bigNumber rem;
	return _dividing(*this, right, rem);
}

bigNumber bigNumber::operator%(const bigNumber& right) const
{
	bigNumber rem;
	_dividing(*this, right, rem);
	return rem;
}

bigNumber& bigNumber::operator^(const bigNumber& right) const
{
	bigNumber* res = new bigNumber(1);
	bigNumber base = *this;
	for (bigNumber i = right; i > (long long int) 0; i = i - 1)
		*res = *res * base;
	return *res;
}


bool bigNumber::operator>(const bigNumber& B)
{
	return this->_compare(B) > 0;
}

bool bigNumber::operator>=(const bigNumber& B)
{
	return this->_compare(B) >= 0;
}

bool bigNumber::operator<(const bigNumber& B)
{
	return this->_compare(B) < 0;
}

bool bigNumber::operator<=(const bigNumber& B)
{
	return this->_compare(B) <= 0;
}

bool bigNumber::operator==(const bigNumber& B)
{
	return this->_compare(B) == 0;
}

bool bigNumber::operator!=(const bigNumber& B)
{
	return this->_compare(B) != 0;
}



char* bigNumber::__str__()
{
	return getString();
}

char* bigNumber::__repr__()
{
	return getString();
}

bigNumber bigNumber::operator+(const int& right) const
{
	return *this + (bigNumber)right;
}

bigNumber bigNumber::operator-(const int& right) const
{
	return *this + (bigNumber)(-right);
}

bigNumber bigNumber::operator*(const int& right) const
{
	return (*this * (bigNumber)right);
}

bigNumber bigNumber::operator/(const int& right) const
{
	bigNumber rem;
	return _dividing(*this, (bigNumber)right, rem);
}

bigNumber bigNumber::operator%(const int& right) const
{
	bigNumber rem;
	_dividing(*this, (bigNumber)right, rem);
	return rem;
}

bool bigNumber::operator>(const int& B)
{
	return this->_compare((bigNumber)B) > 0;
}

bool bigNumber::operator>=(const int& B)
{
	return this->_compare((bigNumber)B) >= 0;
}

bool bigNumber::operator<(const int& B)
{
	return this->_compare((bigNumber)B) < 0;
}

bool bigNumber::operator<=(const int& B)
{
	return this->_compare((bigNumber)B) <= 0;
}

bool bigNumber::operator==(const int& B)
{
	return this->_compare((bigNumber)B) == 0;
}

bool bigNumber::operator!=(const int& B)
{
	return this->_compare((bigNumber)B) != 0;
}