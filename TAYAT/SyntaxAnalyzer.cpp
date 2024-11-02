#include "SyntaxAnalyzer.h"

int SyntaxAnalyzer::look_forward(int pos) {
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
	while (type == typeInt || type == typeShort || type == typeLong || type == typeDouble || type == typeClass) {
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
	if (type == typeInt || type == typeShort || type == typeLong ||  type == typeDouble || type == typeID) {
		data();
		return;
	}
	if (type == typeClass) {
		Tclass();
		return;
	}
	
	type = scan(lex);
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

void SyntaxAnalyzer::element() {
	/*
	type_lex lex;
	int type;
	type = look_forward(1);
	int type2 = look_forward(2);
	if (type == typeID && type2 != TLeftSquareBracket) {
		variable();
		return;
	}
	if (type == typeID && type2 == TLeftSquareBracket) {
		array();
		return;
	}
	else {
		type = scaner->scaner(lex);
		scaner->print_error("Expected element got", lex);
	}*/
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

	type = scan(lex);
	if (type != typeEval)
		scaner->PrintError("Expected = got", lex);

	expression();
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

	type = scan(lex);
	if (type != typeLeftBrace)
		scaner->PrintError("Expected { got", lex);

	operators_and_descriptions();

	type = scan(lex);
	if (type != typeRightBrace)
		scaner->PrintError("Expected } got", lex);
}

void SyntaxAnalyzer::operators_and_descriptions() {
	TypeLex lex;
	int type;

	type = look_forward(1);
	while (type != typeRightBrace) {
		if (type == typeInt || type == typeShort || type == typeLong ||  type == typeDouble) {
			data();
		}
		else operator_();
		type = look_forward(1);

	}
}

void SyntaxAnalyzer::return_statement() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeReturn) {
		scaner->PrintError("Expected return got", lex);
	}

	expression();  // Обработка выражения после return

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
		return_statement();  // Обработка return
		return;
	}

	if (type == typeSemicolon) {
		type = scan(lex);
		return;
	}

	if (type == typeSwitch) {//////////////////////////////
		condition();
		return;
	}

	if (type == typeLeftBrace) {
		composite_operator();
		return;
	}


	int type2 = look_forward(2);
	if (type == typeID && type2 == typeLeftBracket) {
		function_call();
		return;
	}
	if (type == typeID) {
		type2 = look_forward(2);
		if (type2 == typeEval) {  // Проверка на присваивание
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' got", lex);
			return;
		}
	}
	if (type == typeID) {
		type2 = look_forward(3);
		if (type2 == typeEval) {  // Проверка на присваивание
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' got", lex);
			return;
		}
		else if (type2 == typeNew) {  // Обработка создания нового объекта
			type = scan(lex);  // Пропустим 'new'
			function_call();   // Обработка вызова конструктора
			return;
		}
	}


	type = scan(lex);
	scaner->PrintError("Expected operator got", lex);
}

void SyntaxAnalyzer::function_call() {
	TypeLex lex;
	int type;

	type = scan(lex);
	if (type != typeID)
		scaner->PrintError("Expected identificator got", lex);


	type = scan(lex);
	if (type != typeLeftBracket)
		scaner->PrintError("Expected ( got", lex);

	type = scan(lex);
	if (type != typeRightBracket)
		scaner->PrintError("Expected ) got", lex);

	type = scan(lex);
	if (type != typeSemicolon)
		scaner->PrintError("Expected ; got", lex);
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
	/*if (type == TElse)
	{
		type = scan(lex);
		operator_();
	}*/
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
	int type = look_forward(1);

	if (type == typePlus || type == typeMinus)
	{
		type = scan(lex);
		elementary_expression();
	}
	else
		elementary_expression();
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
	else if (type == typeID) {  // Добавьте обработку ID
		type = scan(lex);
		return;
	}

	scaner->PrintError("Expected expression got", lex);
}