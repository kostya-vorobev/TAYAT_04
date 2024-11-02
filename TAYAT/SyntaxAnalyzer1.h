#pragma once
#ifndef __SYNTAXANALYZER
#define __SYNTAXANALYZER

#pragma once
#include "Scanner.h" 
#include "SemantTree.h"

class SyntaxAnalyzer
{
private:
	Scanner* scaner;
	SemanticTree* tree;
public:
	SyntaxAnalyzer(Scanner*, SemanticTree* t);
	void Programm();
private:
	pair<int, string> lookForward(int);
	pair<int, string> Scan();
	void PrintError(string errorMessage, pair<int, string> lexeme);
	void Class();
	void classDesc();
	void Func();
	void funcDesc();
	void Switch();
	void Operator();
	void Const();
	void Data();
	void Assignment();
	void FuncCall();
	TYPE_DATA Sum();
	TYPE_DATA Mul();
	TYPE_DATA UnaryOper();
	TYPE_DATA SimpleOper();
	TYPE_DATA Calculator();

};
#endif