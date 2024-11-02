#pragma once
#include "Scanner.h" 

class SyntaxAnalyzer
{
private:
	Scanner* scaner;
	void description();					// Îïèñàíèå
	void Tclass();
	void classDesc();
	void data();						// Äàííûå
	void function();					// Ôóíêöèÿ
	void return_statement();
	void type();						// Òèï
	void element();
	void list();						// Ñïèñîê
	void variable();					// Ïåðåìåííàÿ
	void assignment();					// Ïðèñâàèâàíèå
	void expression();					// Âûðàæåíèå
	void composite_operator();			// Ñîñòàâíîé îïåðàòîð
	void operators_and_descriptions();	// Îïåðàòîðû è îïèñàíèÿ
	void operator_();					// Îïåðàòîð
	void function_call();				// Âûçîâ ôóíêöèè
	void condition();					// Óñëîâèå
	void comparison();					// Ñðàâíåíèå
	void addendum();					// Ñëàãàåìîå
	void multiplier();					// Ìíîæèòåëü
	void unary_operation();				// Óíàðíàÿ îïåðàöèÿ
	void elementary_expression();		// Ýëåìåíòàðíîå âûðàæåíèå
	int evaluate_expression();			// Âû÷èñëèòü âûðàæåíèå
	int look_forward(int pointer);
	int scan(TypeLex lex);
public:
	SyntaxAnalyzer(Scanner* scaner) {
		this->scaner = scaner;
	}
	~SyntaxAnalyzer() {}
	void program();						// Ïðîãðàììà

};
