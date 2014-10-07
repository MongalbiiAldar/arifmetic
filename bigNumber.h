#pragma once
#include <iostream>
#include <vector>

#define BASE 1000000000

class bigNumber
{
private:
	int _sign;
	std::vector<unsigned int> _elements;

	unsigned int & operator[](int i);
	unsigned int operator[](int i) const;
	void _DelZeroes();
	long long int _compare(const bigNumber& B);
	void _shiftLeft(int s);

	bigNumber _sum(const bigNumber& A, const bigNumber& B) const;
	bigNumber _sub(const bigNumber& A, const bigNumber& B) const;
	bigNumber _dividing(const bigNumber& A, const bigNumber& B, bigNumber &remainder) const;

public:
	bigNumber();
	bigNumber(char* text);
	bigNumber(long long int value);

	char* getString();
	bool getNumberFromTextFile(const char* FileName);
	bool saveNumberToTextFile(const char* FileName);
	bool getNumberFromBinFile(const char* FileName);
	bool saveNumberToBinFile(const char* FileName);


	bigNumber operator+(const bigNumber& right) const;
	bigNumber operator-() const;
	bigNumber operator-(const bigNumber& right) const;
	bigNumber operator*(const bigNumber& right) const;
	bigNumber operator/(const bigNumber& right) const;
	bigNumber operator%(const bigNumber& right) const;
	bigNumber& operator^(const bigNumber& right) const;


	bool operator>(const bigNumber& B);
	bool operator>=(const bigNumber& B);
	bool operator<(const bigNumber& B);
	bool operator<=(const bigNumber& B);
	bool operator==(const bigNumber& B);
	bool operator!=(const bigNumber& B);

	char* __str__();
	char* __repr__();

	bigNumber operator+(const int& right) const;
	bigNumber operator-(const int& right) const;
	bigNumber operator*(const int& right) const;
	bigNumber operator/(const int& right) const;
	bigNumber operator%(const int& right) const;

	bool operator>(const int& B);
	bool operator>=(const int& B);
	bool operator<(const int& B);
	bool operator<=(const int& B);
	bool operator==(const int& B);
	bool operator!=(const int& B);
	
};

bigNumber Pow(const bigNumber& A, const bigNumber& B, bigNumber& modulus);