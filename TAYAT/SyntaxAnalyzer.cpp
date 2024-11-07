#include "SyntaxAnalyzer.h"

int SyntaxAnalyzer::look_forward(int pos) {
	int _ = 0;
	TypeLex lex;
	int saved_pointer = scaner->getPointer();
	int next_type;
	for (int i = 0; i < pos; i++)
		next_type = scaner->Scan(lex);
	scaner->putPointer(saved_pointer);
	return next_type;
}

int SyntaxAnalyzer::scan(TypeLex lex) {
	return scaner->Scan(lex);
}

void SyntaxAnalyzer::program() {
	TypeLex lex;
	int type;
	type = look_forward(1);
	while (type == typeInt || type == typeShort || type == typeLong || type == typeDouble || type == typeClass || type == typeID) {
		description();
		type = look_forward(1);
	}
	if (type != typeEnd) {
		type = scan(lex);
		scaner->PrintError("Expected end got", lex);
	}
}

void SyntaxAnalyzer::Tclass() {
	TypeLex lex;
	int type;

	type = scan(lex);

	if (type != typeClass)
		scaner->PrintError("Waiting class", lex);

	type = scan(lex);

	if (type != typeID)
		scaner->PrintError("Waiting ID", lex);

	type = scan(lex);

	if (type != typeLeftBrace)
		scaner->PrintError("Waiting symbol {", lex);

	type = look_forward(1);

	while (type != typeRightBrace) {
		classDesc();
		type = look_forward(1);
	}


	type = scan(lex);

	if (type != typeRightBrace)
		scaner->PrintError("Waiting symbol }", lex);
	type = scan(lex);
	if (type != typeSemicolon)
		scaner->PrintError("Expected ; got", lex);
}

void SyntaxAnalyzer::classDesc()
{
	TypeLex lex;
	int type;
	type = look_forward(3);
	if (look_forward(1) == typeID && look_forward(2) == typeLeftBracket && look_forward(3) == typeRightBracket && look_forward(4) == typeLeftBrace)
		function();
	if (type == typeLeftBracket) {
		function();
	}
	else data();
}

void SyntaxAnalyzer::description() {
	TypeLex lex;
	int type;
	type = look_forward(3);
	if (type == typeLeftBracket) {
		function();
	}
	type = look_forward(1);
	if (type == typeInt || type == typeShort || type == typeLong || type == typeDouble || type == typeID) {
		data();
		return;
	}
	if (type == typeClass) {
		Tclass();
		return;
	}
	if (type == typeConst) {
		Const();
		return;
	}

	type = scan(lex);
	if (type == typeEnd) {
		type = scan(lex);
		return;

	}
	scaner->PrintError("Expected void or type got", lex);
}

void SyntaxAnalyzer::data() {
	TypeLex lex;
	int type_;

	type();

	list();

	type_ = scan(lex);
	if (type_ != typeSemicolon)
		scaner->PrintError("Expected ; got", lex);
}

void SyntaxAnalyzer::Const()
{
	TypeLex lex;
	int type;
	type = scan(lex);

	if (type != typeConst)
		scaner->PrintError("Waiting const", lex);

	type = look_forward(1);

	if (type == typeInt || type == typeShort || type == typeLong || type == typeDouble)
		data();



}

void SyntaxAnalyzer::function() {
	TypeLex lex;
	int type, pointer;

	type = scan(lex);
	if (type != typeInt && type != typeShort && type != typeLong && type != typeDouble && type != typeID)
		scaner->PrintError("Expected void got", lex);

	type = scan(lex);
	if (type != typeID && type != typeMain)
		scaner->PrintError("Expected identificator got", lex);

	type = scan(lex);
	if (type != typeLeftBracket)
		scaner->PrintError("Expected ( got", lex);

	type = scan(lex);
	if (type != typeRightBracket)
		scaner->PrintError("Expected ) got", lex);

	type = scan(lex);
	if (type != typeLeftBrace)
		scaner->PrintError("Expected { got", lex);

	operators_and_descriptions();

	/*type = look_forward(1);
	if (type != typeReturn)
		scaner->PrintError("Expected return got", lex);

	return_statement();*/

	type = scan(lex);
	if (type != typeRightBrace)
		scaner->PrintError("Expected } got", lex);

}

void SyntaxAnalyzer::type() {
	TypeLex lex;
	int type;
	type = scan(lex);
	if (type != typeInt && type != typeShort && type != typeLong && type != typeDouble && type != typeID)
		scaner->PrintError("Expected type (int, short, long, double) got", lex);
}

void SyntaxAnalyzer::list() {
	TypeLex lex;
	int type, pointer;

	variable();

	type = look_forward(1);

	while (type == typeComma) {
		type = scan(lex);
		variable();
		type = look_forward(1);
	}
}


void SyntaxAnalyzer::variable() {
	TypeLex lex;
	int type;
	type = look_forward(1);
	if (type != typeID) {
		type = scan(lex);
		scaner->PrintError("Expected identificator got", lex);
	}
	else if (look_forward(2) == typePoint) {
		member_access();
	}



	int pointer = scaner->getPointer();
	type = scan(lex);
	scaner->putPointer(pointer);

	type = look_forward(2);

	if (type == typeEval) {
		assignment();
		return;
	}
	type = scan(lex);
}


void SyntaxAnalyzer::assignment() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeID) {
		scaner->PrintError("Expected identificator got", lex);
	}

	type = look_forward(1);


	if (look_forward(1) == typePoint) {
		member_access();
	}

	type = scan(lex);

	if (type != typeEval)
		scaner->PrintError("Expected = got", lex);
	if (look_forward(1) == typeNew)
	{
		type = scan(lex);
		function_call();
	}
	else {
		if (look_forward(2) == typePoint) {
			member_access();
		}
		expression();

	}
}

void SyntaxAnalyzer::expression() {
	TypeLex lex;
	int type;

	comparison();
	type = look_forward(1);
	while (type == typeEq || type == typeNo) {
		type = scan(lex);
		comparison();
		type = look_forward(1);
	}
}

void SyntaxAnalyzer::composite_operator() {
	TypeLex lex;
	int type;

	type = look_forward(1);

	if (type == typeConst)
		Const();
	else
	{
		type = scan(lex);
		if (type != typeLeftBrace)
			scaner->PrintError("Expected { got", lex);

		operators_and_descriptions();

		type = scan(lex);
		if (type != typeRightBrace)
			scaner->PrintError("Expected } got", lex);
	}
}

void SyntaxAnalyzer::operators_and_descriptions() {
	TypeLex lex;
	int type;
	int loops();
	type = look_forward(1);
	while (type != typeRightBrace) {
		if ((type == typeInt || type == typeShort || type == typeLong || type == typeDouble || (type == typeID && look_forward(2) != typeEval && look_forward(2) != typePoint)) && look_forward(3) != typeLeftBracket) {
			data();
		}
		else operator_();
		if (type == typeBreak) {
			scan(lex);
			scan(lex);
			return;
		}
		if (type == typeReturn) {
			return;
		}
		type = look_forward(1);

	}
}

void SyntaxAnalyzer::Switch() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeSwitch)
		scaner->PrintError("Expected 'switch' keyword", lex);

	type = scan(lex);
	if (type != typeLeftBracket)
		scaner->PrintError("Expected '('", lex);
	type = look_forward(1);
	if (type == typeRightBracket) {
		scaner->PrintError("Expected 'not expression'", lex);
		return;
	}
	expression();

	type = scan(lex);
	if (type != typeRightBracket)
		scaner->PrintError("Expected ')'", lex);

	type = scan(lex);
	if (type != typeLeftBrace)
		scaner->PrintError("Expected '{'", lex);

	type = look_forward(1);
	while (type != typeRightBrace) {
		if (type == typeCase) {
			do {
			} while (scan(lex) != typeColon);
			if (look_forward(1) != typeCase)
				operators_and_descriptions();
		}
		else if (type == typeDefault) {
			type = scan(lex);
			if (look_forward(1) == typeColon) {
				scan(lex);
			}
			if (look_forward(1) != typeCase)
				operators_and_descriptions();
		}
		else if (type == typeBreak)
		{
			operators_and_descriptions();
		}
		type = look_forward(1);
	}

	type = scan(lex);
	if (type != typeRightBrace)
		scaner->PrintError("Expected '}'", lex);
}

void SyntaxAnalyzer::member_access() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeID) {
		scaner->PrintError("Expected identifier", lex);
	}

	while (true) {
		type = look_forward(1);
		if (type != typePoint) {
			break;
		}

		scan(lex);

		type = look_forward(1);

		if (type == typeEval) {
			expression();
		}

		if (type != typeID) {
			scaner->PrintError("Expected identifier after '.'", lex);
		}
		type = look_forward(1);
		if (look_forward(2) == typeLeftBracket) {
			function_call();
		}
	}
	type = look_forward(1);
	if (look_forward(1) == typeEval) {
		assignment();
	}
	else {
		expression();
	}
}

void SyntaxAnalyzer::return_statement() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeReturn) {
		scaner->PrintError("Expected return got", lex);
	}

	expression();

	type = scan(lex);
	if (type != typeSemicolon) {
		scaner->PrintError("Expected ; after return statement got", lex);
	}
}

void SyntaxAnalyzer::operator_() {
	TypeLex lex;
	int type;

	type = look_forward(1);
	if (type == typeReturn) {
		return_statement();
		return;
	}

	if (type == typeSemicolon) {
		type = scan(lex);
		return;
	}

	if (type == typeSwitch) {
		Switch();
		return;
	}

	if (type == typeBreak) {
		return;
	}

	if (type == typeConst) {
		composite_operator();
		return;
	}

	if (type == typeLeftBrace) {
		composite_operator();
		return;
	}



	int type2;

	if (type == typeID) {
		type2 = look_forward(2);
		if (type2 == typePoint) {
			member_access();
			return;
		}
	}

	if (type == typeID) {
		type2 = look_forward(2);
		if (type2 == typeLeftBracket) {
			function_call();
			return;
		}
	}

	if (look_forward(2) == typeID) {
		type2 = look_forward(3);
		if (type2 == typeLeftBracket) {
			scan(lex);
			function_call();
			if (look_forward(1) == typeLeftBrace) {
				scan(lex);
				scaner->PrintError("Expected ';' got", lex);
			}
			return;
		}
	}

	if (type == typeID) {
		type2 = look_forward(2);
		if (type2 == typeEval) {
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' got", lex);
			return;
		}
	}
	if (type == typeID) {
		type2 = look_forward(3);
		if (type2 == typeEval) {
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' got", lex);
			return;
		}
		else if (type2 == typeNew) {
			type = scan(lex);
			function_call();
			return;
		}
	}

	if (type == typeRightBracket) {
		return;
	}

	type = scan(lex);
	//scaner->PrintError("Expected operator got", lex);
}

void SyntaxAnalyzer::function_call() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeID) {
		scaner->PrintError("Expected identifier got", lex);
	}

	if (look_forward(1) == typePoint) {
		member_access();
		return;
	}

	type = scan(lex);
	if (type != typeLeftBracket) {
		scaner->PrintError("Expected ( got", lex);
	}

	type = scan(lex);
	if (type != typeRightBracket) {
		scaner->PrintError("Expected ) got", lex);
	}
}

void SyntaxAnalyzer::condition() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeSwitch)
		scaner->PrintError("Expected if got", lex);

	type = scan(lex);
	if (type != typeLeftBracket)
		scaner->PrintError("Expected ( got", lex);

	expression();

	type = scan(lex);
	if (type != typeRightBracket)
		scaner->PrintError("Expected ) got", lex);

	operator_();

	type = look_forward(1);
}

void SyntaxAnalyzer::comparison() {
	TypeLex lex;
	int type;
	addendum();
	type = look_forward(1);
	while (type == typeLess || type == typeLessOrEq || type == typeMore || type == typeMoreOrEq) {
		type = scan(lex);
		addendum();
		type = look_forward(1);
	}
}

void SyntaxAnalyzer::addendum() {
	TypeLex lex;
	int type;

	multiplier();

	type = look_forward(1);
	while (type == typePlus || type == typeMinus) {
		type = scan(lex);
		multiplier();
		type = look_forward(1);
	}
}

void SyntaxAnalyzer::multiplier() {
	TypeLex lex;
	int type;

	unary_operation();

	type = look_forward(1);
	while (type == typeMul || type == typeDiv || type == typeMod) {
		type = scan(lex);
		unary_operation();
		type = look_forward(1);
	}
}

void SyntaxAnalyzer::unary_operation() {
	TypeLex lex;
	int type;

	elementary_expression();

	type = look_forward(1);

	while (type == typePlus || type == typeMinus) {
		scan(lex);
		type = look_forward(2);
		if (type == typePoint) {
			member_access();
			return;
		}
		elementary_expression();
		type = look_forward(1);
	}
}




void SyntaxAnalyzer::elementary_expression() {
	TypeLex lex;
	int type = look_forward(1);

	if (type == typeConst || type == constInt || type == constDouble) {
		type = scan(lex);
		return;
	}
	else if (type == typeLeftBracket) {
		type = scan(lex);
		expression();
		type = scan(lex);
		if (type != typeRightBracket)
			scaner->PrintError("Expected ')' got", lex);
		return;
	}
	else if (type == typeID) {
		if (look_forward(2) == typePoint)
		{
			member_access();
			return;
		}
		type = scan(lex);
		return;
	}
	else if (type == typePoint) {
		type = scan(lex);
		return;
	}
	if (type == typeEnd) {
		return;
	}
}