#pragma once

#ifndef __SCANER
#define __SCANER

#include "LEXEME.h"
#include <iostream>
#include <vector>;
using namespace std;

class Scanner
{
private:
	TypeMod code;
	int pointer;
	int lastLineBreakPosition;
	int currentLine;
public:
	Scanner(FILE* in);
	~Scanner() {}
	int Scan(TypeLex lexeme);
	int getPointer()
	{
		return pointer;
	}
	int getNumberLine()
	{
		return currentLine;
	}
	int getLastLineBreakPosition()
	{
		return lastLineBreakPosition;
	}
	void putPointer(int i)
	{
		pointer = i;
	}
	void setCurrentLine(int line)
	{
		currentLine = line;
	}
	void setLastLineBreakPosition(int pos)
	{
		lastLineBreakPosition = pos;
	}
	void PrintError(string errorMessage, string lexeme);	//Печать ошибок
private:
	void GetData(FILE* in);									//Получение данных

	int parseNumber(TypeLex lexeme, int& i);

    int handleFloatingPoint(TypeLex lexeme, int& i);

    int handleExponent(TypeLex lexeme, int& i);

	vector<int> lineBreakPositions;

};

#endif
