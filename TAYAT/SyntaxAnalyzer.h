#pragma once
#include "Scanner.h" 
#include "SemanticTree.h"

class SyntaxAnalyzer
{
private:
	bool inClass = false;
	Scanner* scaner;
	SemanticTree* semanticTree; // указатель на семантическое дерево
	void description();					
	void Tclass();
	void classDesc();
	void data();						
	void function();					
	void return_statement();
	void type();						
	void list();						
	void variable();					
	void assignment();					
	void expression();					
	void composite_operator();			
	void operators_and_descriptions();	
	void operator_();					
	void function_call();
	void function_call(SemanticTree* semantTree);
	void condition();					
	void comparison();					
	void addendum();					
	void multiplier();					
	void unary_operation();				
	void elementary_expression();		
	void member_access();
	void Switch();
	void Const();
	int look_forward(int pointer);
	int scan(TypeLex lex);
public:

	SyntaxAnalyzer(Scanner* scanner, SemanticTree* tree) : scaner(scanner) , semanticTree(tree) {
		//semanticTree = new SemanticTree(); // инициализация семантического дерева
	}

	~SyntaxAnalyzer() {
		delete semanticTree;
	}

	void program();						// Ïðîãðàììà

};
