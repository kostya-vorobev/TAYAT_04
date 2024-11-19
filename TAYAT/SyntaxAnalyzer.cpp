#include "SyntaxAnalyzer.h"
#include "SemanticTree.h"

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
	inClass = true;
	TypeLex lex;
	int type;

	type = scan(lex);

	if (type != typeClass)
		scaner->PrintError("Waiting class", lex);

	SemanticTree* classNode = NULL;
	TYPE_DATA typeData = semanticTree->getDataType(type);
	type = scan(lex);

	if (type != typeID)
		scaner->PrintError("Waiting ID", lex);

	if (semanticTree->isDoublicateId(semanticTree, lex))
		semanticTree->PrintError("Reassignment", lex);

	Node* newNode = new Node();
	newNode->id = lex;
	newNode->objectType = OBJ_CLASS;
	newNode->dataType = TData();
	if (classNode != NULL) newNode->pointer = classNode;
	semanticTree->setLeft(newNode);
	semanticTree = semanticTree->getLeft();
	semanticTree->setRight(NULL);
	SemanticTree* tmpTree = semanticTree;
	semanticTree = semanticTree->getRight();

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
	semanticTree = tmpTree;
	inClass = false;
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

	type_ = look_forward(3);
	if (type_ == typeLeftBracket) {
		function();
		return;
	}

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


	Node* newNode = new Node();
	newNode->id = lex;
	newNode->objectType = OBJ_CONST;
	//newNode->dataType = semanticTree->semantic_include(lex, OBJ_CONST, semanticTree->getDataType(type));
	newNode->flagInit = 1; // ������������� ���� �������������
	semanticTree->setLeft(newNode);
	//semanticTree = semanticTree->getLeft();
}

void SyntaxAnalyzer::function() {
	TypeLex lex;
	int type, pointer;
	Node* newNode = new Node();
	newNode->dataType = TData();
	type = scan(lex);
	if (type != typeInt && type != typeShort && type != typeLong && type != typeDouble && type != typeID) {
		scaner->PrintError("Expected return type got", lex);
	}
	newNode->dataType.dataType = semanticTree->getDataType(type);
	newNode->objectType = OBJ_FUNC;
	// ��������� ��� �����, � ������� ���������
	SemanticTree* classNode = (inClass) ? semanticTree->findUp(lex) : nullptr;

	// �������� �� ������������� �������
	type = scan(lex);
	if (type != typeID && type != typeMain) {
		scaner->PrintError("Expected identifier got", lex);
	}

	// �������� �� ������������ ��������������
	if (semanticTree->isDoublicateId(semanticTree, lex)) {
		scaner->PrintError("Reassignment", lex);
	}

	// ������� ����� ���� ��� �������

	newNode->id = lex;  // ��������� �������������
	//newNode->dataType = semanticTree->getDataType(type);
	newNode->classPointer = classNode; // ��������� �� ������������ �����, ���� ������� ������ ������

	// ���������� ���� � ������
	semanticTree->setLeft(newNode);
	semanticTree = semanticTree->getLeft(); // ������� � ������ ����
	semanticTree->setRight(NULL);

	SemanticTree* tmpTree = semanticTree; // ��������� ������� ��������� ������

	semanticTree = semanticTree->getRight(); // ������� � ������� ���������

	// �������� �� ������ ���� �������
	type = scan(lex);
	if (type != typeLeftBracket) {
		scaner->PrintError("Expected ( got", lex);
	}

	// �������� �� ����������� ������� ������
	type = scan(lex);
	if (type != typeRightBracket) {
		scaner->PrintError("Expected ) got", lex);
	}

	// �������� �� ������ ����� �������
	type = scan(lex);
	if (type != typeLeftBrace) {
		scaner->PrintError("Expected { got", lex);
	}

	// ��������� ���������� (���� �������)
	operators_and_descriptions();

	// �������� � ����������� ����
	semanticTree = tmpTree;

	// �������� �� ����������� �������� ������
	type = scan(lex);
	if (type != typeRightBrace) {
		scaner->PrintError("Expected } got", lex);
	}
}

void SyntaxAnalyzer::type() {
	TypeLex lex;
	int type;
	type = look_forward(1);
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
	Node* newNode = new Node();
	newNode->dataType = TData();
	type = look_forward(1);
	if (type == typeInt || type == typeShort || type == typeLong || type == typeDouble)
	{
		type = scan(lex);
		newNode->dataType.dataType = semanticTree->getDataType(type);
		newNode->objectType = OBJ_VAR;
	}
	else
		if (type == typeID) {
			type = scan(lex);		
			newNode->dataType.dataType = semanticTree->getDataType(type);
			newNode->flagInit = 1;
			newNode->classPointer = semanticTree->findUp(lex);
			newNode->objectType = OBJ_CLASS_OBJ;
		}


	type = look_forward(1);
	SemanticTree* varNode = NULL;
	TYPE_DATA typeData = semanticTree->getDataType(type);
	if (type != typeID) {
		type = scan(lex);
		scaner->PrintError("Expected identificator got", lex);
	}
	else if (look_forward(2) == typePoint) {
		member_access();
	}



	int pointer = scaner->getPointer();

	type = scan(lex);
	// ��������� ������������ ��������������
	if (semanticTree->isDoublicateId(semanticTree, lex)) {
		semanticTree->PrintError("Reassignment", lex);
	}

	// ������ ����� ���� ��� ���������� � ������

	newNode->id = lex;  // ��������� �������������

	type = look_forward(1);
	if (type == typeEval) {
		newNode->flagInit = 1;
	}
	else
	{
		newNode->flagInit = 0;
	}

	// ��������� ���� � ������
	semanticTree->setLeft(newNode);

	// ������� � ������ ��������� ���� ��� ���������� ��������
	semanticTree = semanticTree->getLeft();

	// ������������� ������� ������ ����� ���������� ����


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

	SemanticTree* node = semanticTree->findUp(lex); // ���� ���� �� ��������������

	if (node == NULL) {
		scaner->PrintError("Semant Error. ID is not found", lex);
	}
	node->setInit();
	// ����� �������� ����
	TYPE_DATA varType = node->getSelfDataType();

	type = look_forward(1);

	if (look_forward(1) == typePoint) {
		member_access();
	}

	type = scan(lex);

	if (type != typeEval) {
		scaner->PrintError("Expected = got", lex);
	}


	TData* val = expression();
	node->setValue(node->getSelfId(), val->value);
}


TData* SyntaxAnalyzer::expression() {
	TypeLex lex;
	TData* result = new TData();
	int type;

	result = comparison();
	type = look_forward(1);
	while (type == typeEq || type == typeNo) {
		type = scan(lex);
		if (type == typeEq)
			result->value.dataDouble = (result->value.dataDouble == comparison()->value.dataDouble);
		if (type == typeLessOrEq)
			result->value.dataDouble = !(result->value.dataDouble == comparison()->value.dataDouble);
		type = look_forward(1);
	}
	return result;
}

void SyntaxAnalyzer::composite_operator() {
	TypeLex lex;
	int type;

	type = look_forward(1);
	SemanticTree* varNode = NULL;
	TYPE_DATA typeData = semanticTree->getDataType(type);
	if (type == typeConst)
		Const();
	else
	{
		type = scan(lex);
		if (type != typeLeftBrace)
			scaner->PrintError("Expected { got", lex);

		Node* newNode = new Node();
		newNode->dataType = TData();
		newNode->id = lex;
		newNode->objectType = OBJ_VAR;
		newNode->dataType.dataType = semanticTree->getDataType(type);
		//newNode->dataType = typeData;
		if (varNode != NULL) newNode->pointer = varNode;
		semanticTree->setLeft(newNode);
		semanticTree = semanticTree->getLeft();
		semanticTree->setRight(NULL);
		SemanticTree* tmpTree = semanticTree;

		semanticTree = semanticTree->getRight();

		operators_and_descriptions();

		type = scan(lex);
		if (type != typeRightBrace)
			scaner->PrintError("Expected } got", lex);
		semanticTree = tmpTree;
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

TData* SyntaxAnalyzer::member_access() {
	TypeLex lex;
	int type;
	TData* result = new TData;;

	type = scan(lex);
	if (type != typeID) {
		scaner->PrintError("Expected identifier", lex);
	}

	// ���� ����, ��������������� �������
	SemanticTree* objectNode = semanticTree->findUp(lex);
	
	if (objectNode == NULL){
		objectNode = semanticTree->getClassPointer();
		objectNode = objectNode->findRightLeft(lex);
		if (objectNode == NULL) {
			scaner->PrintError("Object not found", lex);
				return result;
		}
		semanticTree = objectNode;
	}



	// ���������, ��� ���� �������� �������
	if (objectNode->getSelfObjectType() == OBJ_CLASS_OBJ) {
		// �������� ��� ������
		type = look_forward(1);
		if (type != typePoint) {
			return result; // ���� �� �����, ������� �� �����
		}

		scan(lex); // ���������� �����
		type = look_forward(2); // ��������� ��� ������ ���� ���������������
		if (type == typePoint) {
			result = member_access();
			semanticTree = objectNode;
			return result; // ����� �� ������� ��� ������
		}

		type = look_forward(1); // ��������� ��� ������ ���� ���������������
		if (type != typeID) {
			
			return result; // ����� �� ������� ��� ������
		}

		// �������� ������� ������ � ������
		SemanticTree* methodNode = objectNode->getNode();
		if (methodNode == nullptr) {
			scaner->PrintError("Method not found", lex);
			//return result;
		}
		else
			if (look_forward(2) == typeLeftBracket)
				function_call(methodNode);
	}

	// ��������� �� ���������
	type = look_forward(1);
	if (type == typeID)
	{
		SemanticTree* methodNode = objectNode->getNode();
		if (methodNode == nullptr) {
			scaner->PrintError("identifier not found in class", lex);
			//return result;
		}
		else {
			scan(lex);
			SemanticTree* idNode = methodNode->findRightLeft(lex);
			if (idNode == NULL) {
				scaner->PrintError("identifier not found in class", lex);
				//return; // ����� �� ������� ��� ������
			}
			if (idNode->isSelfInit() == 0) {
				scaner->PrintError("Semant Error. Variable is not initialized", lex);
				//return; // ����� �� ������� ��� ������
			}
			result = idNode->getData();
			type = look_forward(1);
		}
	}
	if (type == typeEval) {
		assignment();
	}
	return result;
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
				scaner->PrintError("Expected ';' 1 got", lex);
			}
			return;
		}
	}

	if (type == typeID) {
		type2 = look_forward(1);
		if (type2 == typeEval) {
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' 4 got", lex);
			return;
		}
	}

	if (type == typeID) {
		type2 = look_forward(2);
		if (type2 == typeEval) {
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' 2 got", lex);
			return;
		}
	}
	if (type == typeID) {
		type2 = look_forward(3);
		if (type2 == typeEval) {
			assignment();
			type = scan(lex);
			if (type != typeSemicolon)
				scaner->PrintError("Expected ';' 3 got", lex);
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
	// ���� ����, ��������������� �������
	SemanticTree* objectNode = semanticTree->findUp(lex);
	// ����� ��������� ����� ������
	SemanticTree* methodNode = objectNode->findMethod(lex); // ����� ��� ������ �� ����� ������ � ������ �� findUp
	if (methodNode == NULL) {
		scaner->PrintError("Method not found", lex);
		return; // ����� �� ������� ��� ������
	}

	// ���������, ��� ���� �������� ��������
	if (methodNode->getSelfObjectType() != OBJ_FUNC) {
		scaner->PrintError("Not a method", lex);
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

void SyntaxAnalyzer::function_call(SemanticTree* semantTree) {
	TypeLex lex;
	int type;

	type = scan(lex);

	if (type != typeID) {
		scaner->PrintError("Expected identifier got", lex);
	}
	// ���� ����, ��������������� �������
	SemanticTree* objectNode = semantTree->findRightLeft(lex);
	// ����� ��������� ����� ������
	SemanticTree* methodNode = objectNode->findMethod(lex); // ����� ��� ������ �� ����� ������ � ������ �� findUp
	if (methodNode == NULL) {
		scaner->PrintError("Method not found", lex);
		return; // ����� �� ������� ��� ������
	}

	// ���������, ��� ���� �������� ��������
	if (methodNode->getSelfObjectType() != OBJ_FUNC) {
		scaner->PrintError("Not a method", lex);
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
		scaner->PrintError("Expected Switch got", lex);

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

TData* SyntaxAnalyzer::comparison() {
	TypeLex lex;
	int type;
	TData* result = new TData;;

	result = addendum();

	type = look_forward(1);
	while (type == typeLess || type == typeLessOrEq || type == typeMore || type == typeMoreOrEq) {
		type = scan(lex);
		if (type == typeLess)
			result->value.dataDouble = (result->value.dataDouble < addendum()->value.dataDouble);
		if (type == typeLessOrEq)
			result->value.dataDouble = (result->value.dataDouble <= addendum()->value.dataDouble);
		if (type == typeMore)
			result->value.dataDouble = (result->value.dataDouble > addendum()->value.dataDouble);
		if (type == typeMoreOrEq)
			result->value.dataDouble = (result->value.dataDouble >= addendum()->value.dataDouble);
		type = look_forward(1);
	}
	return result;
}

TData* SyntaxAnalyzer::addendum() {
	TypeLex lex;
	int type;
	TData* result = new TData;;

	result = multiplier();

	type = look_forward(1);
	while (type == typePlus || type == typeMinus) {
		type = scan(lex);
		if (type == typePlus)
			result->value.dataDouble += multiplier()->value.dataDouble;
		if (type == typeMinus)
			result->value.dataDouble -= multiplier()->value.dataDouble;
		type = look_forward(1);
	}
	return result;
}

TData* SyntaxAnalyzer::multiplier() {
	TypeLex lex;
	int type;
	TData* result = new TData;;

	result = unary_operation();

	type = look_forward(1);
	while (type == typeMul || type == typeDiv || type == typeMod) {
		type = scan(lex);
		if (type == typeMul) {
			TData* buffer = unary_operation();
			result->dataType = buffer->dataType;
			result->value.dataDouble *= buffer->value.dataDouble;
		}
		if (type == typeDiv) {
			TData* buffer = unary_operation();
			if (buffer->dataType == TYPE_INTEGER && result->dataType == TYPE_INTEGER && result->value.dataDouble!=0) {
				result->value.dataDouble = (int)(result->value.dataDouble / buffer->value.dataDouble);
			}
			else
				if (buffer->value.dataDouble != 0)
					result->value.dataDouble /= buffer->value.dataDouble;
				else
					scaner->PrintError("Zero devision.", lex);
		}
		if (type == typeMod) {
			TData* buffer = elementary_expression();
			result->dataType = buffer->dataType;
			result->value.dataDouble = fmod(result->value.dataDouble, buffer->value.dataDouble);
		}
		type = look_forward(1);
	}
	return result;
}

TData* SyntaxAnalyzer::unary_operation() {
	TypeLex lex;
	int type, type2;
	TData* result = new TData;

	

	type = look_forward(1);


		if (type == typePlus) {
			type = scan(lex);
			TData* buffer = elementary_expression();
			result->dataType = buffer->dataType;
			result->value.dataDouble += buffer->value.dataDouble;
		}else
		if (type == typeMinus) {
			type = scan(lex);
			TData* buffer = elementary_expression();
			result->dataType = buffer->dataType;
			result->value.dataDouble -= buffer->value.dataDouble;
		}else 
			result = elementary_expression();

		type = look_forward(1);
	
	return result;
}




TData* SyntaxAnalyzer::elementary_expression() {
	TypeLex lex;
	TData* res = new TData();
	TData* result = new TData;
	int type = look_forward(1);

	if (type == typeConst || type == constInt || type == constDouble) {
		type = scan(lex);
		switch (type) {
		case typeConst:
			result->value.dataInt = 0;
			return result;
			break;
		case constInt:
			
				result->value.dataDouble= std::stod(string(lex));
				result->dataType = TYPE_INTEGER;
				//res->value.dataInt = std::stoi(string(lex));
			return result;
			break;
		case constDouble:
			try {
				result->value.dataDouble = std::stod(lex);
				result->dataType = TYPE_DOUBLE;
			}
			catch (const std::invalid_argument&) {
				scaner->PrintError("Invalid value for integer conversion.", lex);
			}
			catch (const std::out_of_range&) {
				scaner->PrintError("Integer value out of range.", lex);
			}
			return result;
			break;
		}
	}
	else if (type == typeLeftBracket) {
		type = scan(lex);
		result = expression();
		type = scan(lex);
		if (type != typeRightBracket)
			scaner->PrintError("Expected ')' got", lex);
		return result;
	}
	else if (type == typeID) {
		/*
		// �������� ������� ����� � �������������� �����*/
		if (look_forward(2) == typePoint) {
			result = member_access();
			return result;
		}

		if (look_forward(2) == typeLeftBracket) {
			function_call();
			return result;
		}
		type = scan(lex); // ��������� �������������
		SemanticTree* node = semanticTree->findUp(lex);
		if (node == nullptr) {
			scaner->PrintError("Semant Error. Variable not found", lex);
			//semanticTree->PrintError("Variable is not initialized", lex);
		}
		result = node->getData();
		return result;
	}
	else if (type == typePoint) {
		type = scan(lex);
		return result;
	}
	if (type == typeEnd) {
		return result;
	}
	return result;

}
/*
TYPE_VALUE SyntaxAnalyzer::elementary_expression(SemanticTree* tree) {
	TypeLex lex;
	TYPE_VALUE result;
	int type = look_forward(1);

	if (type == typeConst || type == constInt || type == constDouble) {
		type = scan(lex);
		switch (type) {
		case typeConst:
			result.dataInt = 0;
			return result;
			break;
		case constInt:
			try {
				result.dataInt = std::stoi(lex);
			}
			catch (const std::invalid_argument&) {
				scaner->PrintError("Invalid value for integer conversion.", lex);
			}
			catch (const std::out_of_range&) {
				scaner->PrintError("Integer value out of range.", lex);
			}
			return result;
			break;
		case constDouble:
			try {
				result.dataInt = std::stod(lex);
			}
			catch (const std::invalid_argument&) {
				scaner->PrintError("Invalid value for integer conversion.", lex);
			}
			catch (const std::out_of_range&) {
				scaner->PrintError("Integer value out of range.", lex);
			}
			return result;
			break;
		}
	}
	else if (type == typeLeftBracket) {
		type = scan(lex);
		result = expression();
		type = scan(lex);
		if (type != typeRightBracket)
			scaner->PrintError("Expected ')' got", lex);
		return result;
	}
	else if (type == typeID) {
		type = scan(lex); // ��������� �������������
		SemanticTree* node = semanticTree->findUp(lex);
		if (node == nullptr) {
			scaner->PrintError("Semant Error. Variable not found", lex);
			//semanticTree->PrintError("Variable is not initialized", lex);
		}
		if (node->getSelfObjectType() == OBJ_CLASS_OBJ) {
			scaner->PrintError("Semant Error. Cannot use class object", lex);
			//semanticTree->PrintError("Variable is not initialized", lex);
		}
		if (node->isSelfInit() == 0) {
			scaner->PrintError("Semant Error. Variable is not initialized", lex);
			//semanticTree->PrintError("Variable is not initialized", lex);
		}
		// �������� ������� ����� � �������������� �����
		if (look_forward(1) == typePoint) {
			member_access();
			return;
		}

		if (look_forward(1) == typeLeftBracket) {
			function_call();
			return;
		}

		return result;
	}
	else if (type == typePoint) {
		type = scan(lex);
		return result;
	}
	if (type == typeEnd) {
		return result;
	}

}*/