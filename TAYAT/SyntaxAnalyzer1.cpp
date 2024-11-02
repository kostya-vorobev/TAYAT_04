#define _CRT_SECURE_NO_WARNINGS
#include "SyntaxAnalyzer.h"
#include "SemantTree.h"

bool fromSwitch = false;

SyntaxAnalyzer::SyntaxAnalyzer(Scanner* s, SemanticTree* t)
{
	scaner = s;
	tree = t;
}

pair<int, string> SyntaxAnalyzer::lookForward(int numberOfStep) 
{
	int t; TypeLex l;
	int savedPointer = scaner->getPointer();
	int savedLine = scaner->getNumberLine();
	for (int i = 0; i < numberOfStep; i++) {
		t = scaner->Scan(l);
	}
	scaner->putPointer(savedPointer);
	scaner->setCurrentLine(savedLine);
	pair<int, string> lexeme(t, l);
	return lexeme;
}

pair<int, string> SyntaxAnalyzer::Scan()
{
	int t; TypeLex l;
	t = scaner->Scan(l);
	pair<int, string> lexeme(t, l);
	return lexeme;
}

void PrintError(string errorMessage, pair<int, string> lexeme)
{
	cout << "Syntax Error. " << errorMessage << " Find: " << lexeme.second << endl;
	exit(1);
}

void SyntaxAnalyzer::Programm()
{
	pair<int, string> lexeme = lookForward(1);

	while (lexeme.first != typeEnd) {
		if (lexeme.first == typeClass) {
			Class();
		}
		else if (lexeme.first == typeConst) {
			Const();
		}
		else {
			lexeme = lookForward(3);

			if (lexeme.first == typeLeftBracket) {
				Func();
			}
			else {
				Data();
			}
		}
		lexeme = lookForward(1);
	}
}

void SyntaxAnalyzer::Class() 
{
	pair<int, string> lexeme = Scan();

	if (lexeme.first != typeClass) 
		PrintError("Waiting class", lexeme);

	lexeme = Scan();

	if (lexeme.first != typeID) 
		PrintError("Waiting ID", lexeme);

	if (tree->isDoublicateId(tree, lexeme.second))
		tree->PrintError("Reassignment", lexeme);

	Node* newNode = new Node();
	newNode->id = lexeme.second;
	newNode->objectType = OBJ_CLASS;
	tree->setLeft(newNode);
	tree = tree->getLeft();
	tree->setRight(NULL);
	SemanticTree* tmpTree = tree;
	tree = tree->getRight();

	lexeme = Scan();

	if (lexeme.first != typeLeftBrace) 
		PrintError("Waiting symbol {", lexeme);

	lexeme = lookForward(1);

	while (lexeme.first != typeRightBrace) {
		classDesc();
		lexeme = lookForward(1);
	}

	tree = tmpTree;

	lexeme = Scan();

	if (lexeme.first != typeRightBrace)
		PrintError("Waiting symbol }", lexeme);
}

void SyntaxAnalyzer::classDesc() 
{
	pair<int, string> lexeme = lookForward(3);

	if (lexeme.first == typeLeftBracket) {
		Func();
	}
	else Data();
}

void SyntaxAnalyzer::Func()
{
	pair<int, string> lexeme = Scan();

	if (lexeme.first != typeShort && lexeme.first != typeLong && lexeme.first != typeInt && lexeme.first != typeDouble && lexeme.first != typeID)
		PrintError("Waiting type", lexeme);

	SemanticTree* classNode = NULL;
	TYPE_DATA type = tree->getDataType(lexeme.first);
	if (type == TYPE_UNKNOWN)
	{
		classNode = tree->findUp(lexeme.second);
		if (classNode == NULL) 
			tree->PrintError("Class in not found", lexeme);
	}
	lexeme = Scan();

	if (lexeme.first != typeID && lexeme.first != typeMain)
		PrintError("Waiting ID or main", lexeme);

	if (tree->isDoublicateId(tree, lexeme.second))
		tree->PrintError("Reassignment", lexeme);

	Node* newNode = new Node();
	newNode->id = lexeme.second;
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = type;
	if (classNode != NULL) newNode->pointer = classNode;
	tree->setLeft(newNode);
	tree = tree->getLeft();
	tree->setRight(NULL);
	SemanticTree* tmpTree = tree;
	tree = tree->getRight();

	lexeme = Scan();
	if (lexeme.first != typeLeftBracket) 
		PrintError("Waiting symbol (", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeRightBracket) 
		PrintError("Waiting symbol )", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeLeftBrace) 
		PrintError("Waiting symbol {", lexeme);

	lexeme = lookForward(1);

	while (lexeme.first != typeRightBrace) {
		funcDesc();
		lexeme = lookForward(1);
	}
	tree = tmpTree;

	lexeme = Scan();
	if (lexeme.first != typeRightBrace) 
		PrintError("Waiting symbol }", lexeme);
}

void SyntaxAnalyzer::funcDesc()
{
	pair<int, string> lexeme = lookForward(1);

	if (lexeme.first == typeClass)
		Class();
	else if (lexeme.first == typeSwitch)
		Switch();
	else if (lexeme.first == typeConst)
		Const();
	else if (lexeme.first == typeBreak || lexeme.first == typeReturn)
		Operator();
	else if (lexeme.first == typeLong || lexeme.first == typeShort || lexeme.first == typeInt || lexeme.first == typeDouble)
		Data();
	else if (lexeme.first == typeID)
	{
		int tmpIndex = scaner->getPointer();

		lexeme = Scan();
		lexeme = lookForward(1);

		while (lexeme.first == typePoint) {
			lexeme = Scan();
			lexeme = Scan();
			if (lexeme.first != typeID)
				PrintError("Waiting identifier", lexeme);
			lexeme = lookForward(1);
		}

		scaner->putPointer(tmpIndex);
		if (lexeme.first == typeLeftBracket)
			FuncCall();
		else if (lexeme.first == typeEval || lexeme.first == typeMinusEq || lexeme.first == typePlusEq || lexeme.first == typeMulEq || lexeme.first == typeDivEq || lexeme.first == typeModEq)
			Assignment();
		else
			Data();
	}
	else if (lexeme.first == typeBreak || lexeme.first == typeReturn)
		Operator();
	else 
	{
		Calculator();
		lexeme = Scan();
		if (lexeme.first != typeSemicolon) 
			PrintError("Waiting ;", lexeme);
	}
}

void SyntaxAnalyzer::Switch()
{
	pair<int, string> lexeme = Scan();
	if (lexeme.first != typeSwitch)
		PrintError("Waiting switch", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeLeftBracket)
		PrintError("Waiting (", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeID)
		PrintError("Waiting ID", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeRightBracket)
		PrintError("Waiting )", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeLeftBrace)
		PrintError("Waiting {", lexeme);

	lexeme = lookForward(1);
	while (lexeme.first != typeRightBrace)
	{
		if (lexeme.first == typeBreak)
			do {
				lexeme = Scan();
			} while (lexeme.first != typeCase && lexeme.first != typeDefault);
		funcDesc();
		lexeme = lookForward(1);
	}
	lexeme = Scan();
	if (lexeme.first != typeRightBrace)
		PrintError("Waiting }", lexeme);
}

void SyntaxAnalyzer::Operator()
{
	pair<int, string> lexeme = Scan();
	if (lexeme.first != typeBreak && lexeme.first != typeReturn)
		PrintError("Waiting return or break", lexeme);

	if (lexeme.first == typeReturn)
	{
		pair<int, string> funcName = lookForward(1);
		TYPE_DATA calcType = Calculator();

		SemanticTree* funcNode = tree->findUp(OBJ_FUNC);
		if (funcNode == NULL)
			tree->PrintError("Operation out of function", lexeme);

		TYPE_DATA funcType = funcNode->getSelfDataType();

		if (funcType == TYPE_UNKNOWN) {
			SemanticTree* foundedNode = tree->findUp(funcName.second);

			if (foundedNode == NULL)
				tree->PrintError("TypeError", lexeme);

			if (foundedNode->getSelfObjectType() == OBJ_FUNC) {
				foundedNode = foundedNode->getClassPointer();
			}

			if (foundedNode == NULL)
				tree->PrintError("TypeError", lexeme);

			if (funcNode->getClassPointer() != foundedNode)
				tree->PrintError("TypeError", lexeme);
		}

		if (calcType > funcType)
			tree->PrintError("TypeError", lexeme);
	}

	lexeme = Scan();
	if (lexeme.first != typeSemicolon)
		PrintError("Waiting symbol ;", lexeme);
}

void SyntaxAnalyzer::Const()
{
	pair<int, string> lexeme = Scan();

	if (lexeme.first != typeConst) 
		PrintError("Waiting const", lexeme);

	lexeme = Scan();

	if (lexeme.first != typeInt && lexeme.first != typeShort && lexeme.first != typeLong && lexeme.first != typeDouble)
		PrintError("Waiting type", lexeme);

	int type = lexeme.first;

	lexeme = Scan();

	if (lexeme.first != typeID) 
		PrintError("Waiting ID", lexeme);
	if (tree->isDoublicateId(tree, lexeme.second))
		tree->PrintError("Reassignment", lexeme);

	string id = lexeme.second;

	lexeme = Scan();
	if (lexeme.first != typeEval) 
		PrintError("Waiting =", lexeme);

	lexeme = Scan();
	if (lexeme.first != constInt && lexeme.first != constDouble)
		PrintError("Waiting number", lexeme);

	string data = lexeme.second;

	lexeme = Scan();

	if (lexeme.first != typeSemicolon) 
		PrintError("Waiting ;", lexeme);

	Node* newNode = new Node();
	newNode->id = id;
	newNode->objectType = OBJ_CONST;
	newNode->dataType = tree->getDataType(type);
	tree->setLeft(newNode);
	tree = tree->getLeft();
}

void SyntaxAnalyzer::Data()
{
	pair<int, string> lexeme = Scan();

	if (lexeme.first != typeInt && lexeme.first != typeShort && lexeme.first != typeLong && lexeme.first != typeDouble && lexeme.first != typeID)
		PrintError("Waiting type", lexeme);

	bool isObject = lexeme.first == typeID;
	pair<int, string> type = lexeme;

	do {
		lexeme = Scan();

		if (lexeme.first != typeID) 
			PrintError("Waiting ID", lexeme);

		if (tree->isDoublicateId(tree, lexeme.second))
			tree->PrintError("Reassignment", lexeme);

		string id = lexeme.second;

		lexeme = Scan();

		int init = 0;

		if (lexeme.first == typeEval) 
		{
			pair<int, string> funcName = lookForward(1);
			TYPE_DATA calcType = Calculator();

			if (isObject)
			{
				SemanticTree* foundedNode = tree->findUp(funcName.second);

				if (foundedNode == NULL)
					tree->PrintError("TypeError", lexeme);

				if (foundedNode->getSelfObjectType() == OBJ_FUNC || foundedNode->getSelfObjectType() == OBJ_CLASS_OBJ) 
					foundedNode = foundedNode->getClassPointer();
				if (foundedNode == NULL)
					tree->PrintError("TypeError",lexeme);
				if (type.second != foundedNode->getSelfId())
					tree->PrintError("TypeError", lexeme);
			}
			else
			{
				TYPE_DATA varType = tree->getDataType(type.first);
				if (calcType == TYPE_UNKNOWN || calcType > varType)
					tree->PrintError("TypeError", lexeme);
			}
			init = 1;
			lexeme = Scan();
		}
		Node* newNode = new Node();
		newNode->id = id;
		if (isObject) {
			newNode->objectType = OBJ_CLASS_OBJ;
			SemanticTree* classNode = tree->findUp(type.second, OBJ_CLASS);
			if (classNode == NULL)
				tree->PrintError("Class not found", lexeme);
			newNode->pointer = classNode;
		}
		else {
			newNode->objectType = OBJ_VAR;
			newNode->dataType = tree->getDataType(type.first);
		}
		newNode->flagInit = init;
		tree->setLeft(newNode);
		tree = tree->getLeft();
	} while (lexeme.first == typeComma);

	if (lexeme.first != typeSemicolon)
		PrintError("Waiting ;", lexeme);
}

void SyntaxAnalyzer::Assignment()
{
	pair<int, string> lexeme = Scan();

	if (lexeme.first != typeID) 
		PrintError("Waiting ID", lexeme);


	SemanticTree* node = tree->findUp(lexeme.second);

	if (node == NULL)
		tree->PrintError("ID is not found", lexeme);
	TYPE_OBJECT foundedNodeType = node->getSelfObjectType();
	if (foundedNodeType == OBJ_CLASS_OBJ) node = node->getClassPointer();
	else if (foundedNodeType != OBJ_VAR)
		PrintError("Restricted assignment", lexeme);

	lexeme = lookForward(1);

	while (lexeme.first == typePoint) 
	{
		lexeme = Scan();
		lexeme = Scan();
		if (lexeme.first != typeID) 
			PrintError("Waiting ID ", lexeme);

		if (node == NULL)
			tree->PrintError("ID is not found", lexeme);
		foundedNodeType = node->getSelfObjectType();
		if (foundedNodeType == OBJ_CLASS_OBJ) node = node->getClassPointer();
		else if (foundedNodeType != OBJ_VAR) 
			PrintError("Restricted assignment", lexeme);
		lexeme = lookForward(1);
	}

	if (lexeme.first != typeEval && lexeme.first != typePlusEq && lexeme.first != typeMinusEq && lexeme.first != typeMulEq && lexeme.first != typeDivEq && lexeme.first != typeModEq) 
		PrintError("Waiting =", lexeme);
	lexeme = Scan();

	TYPE_DATA type = Calculator();

	if (foundedNodeType == OBJ_CLASS) {
		string nodeId = node->getSelfId();

		if (nodeId != lookForward(1).second)
			tree->PrintError("TypeError", lexeme);
	}
	else {
		TYPE_DATA nodeDataType = node->getSelfDataType();

		if (nodeDataType == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		if (type == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		if (type > nodeDataType)
			tree->PrintError("TypeError", lexeme);
	}

	lexeme = Scan();

	if (lexeme.first != typeSemicolon) {
		PrintError("Waiting ;", lexeme);
	}
}

void SyntaxAnalyzer::FuncCall()
{
	pair<int, string> lexeme = Scan();

	if (lexeme.first != typeID)
		PrintError("Waiting ID", lexeme);

	SemanticTree* node = tree->findUp(lexeme.second);

	if (node == NULL)
		tree->PrintError("ID is not found", lexeme);
	TYPE_OBJECT foundedNodeType = node->getSelfObjectType();
	if (foundedNodeType == OBJ_CLASS_OBJ) node = node->getClassPointer();
	else if (foundedNodeType != OBJ_FUNC)
		tree->PrintError("Not a function", lexeme);

	lexeme = lookForward(1);
	while (lexeme.first == typePoint) {
		lexeme = Scan();
		lexeme = Scan();
		if (lexeme.first != typeID) 
			PrintError("Waiting ID", lexeme);

		node = node->findRightLeft(lexeme.second);

		if (node == NULL)
			tree->PrintError("ID is not found", lexeme);
		foundedNodeType = node->getSelfObjectType();
		if (foundedNodeType == OBJ_CLASS_OBJ) node = node->getClassPointer();
		else if (foundedNodeType != OBJ_FUNC)
			tree->PrintError("Not a function", lexeme);

		lexeme = lookForward(1);
	}

	lexeme = Scan();
	if (lexeme.first != typeLeftBracket) 
		PrintError("Waiting (", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeRightBracket) 
		PrintError("Waiting )", lexeme);

	lexeme = Scan();
	if (lexeme.first != typeSemicolon) 
		PrintError("Waiting ;", lexeme);
}

TYPE_DATA SyntaxAnalyzer::Sum() 
{
	TYPE_DATA type = Mul();

	pair<int, string> lexeme = lookForward(1);

	while (lexeme.first == typePlus || lexeme.first == typeMinus) 
	{
		lexeme = Scan();
		int operation = lexeme.first;
		TYPE_DATA type2 = Mul();
		lexeme = lookForward(1);

		if (type == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		if (type2 == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		type = tree->getTypeResult(type, type2, operation);
		if (type == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
	}
	return type;
}

TYPE_DATA SyntaxAnalyzer::Mul()
{
	TYPE_DATA type = UnaryOper();

	pair<int, string> lexeme = lookForward(1);

	while (lexeme.first == typeMod || lexeme.first == typeDiv || lexeme.first == typeMul || lexeme.first == typeMod) 
	{
		lexeme = Scan();
		int operation = lexeme.first;
		TYPE_DATA type2 = UnaryOper();
		lexeme = lookForward(1);

		if (type == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		if (type2 == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		type = tree->getTypeResult(type, type2, operation);
		if (type == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
	}

	return type;
}

TYPE_DATA SyntaxAnalyzer::UnaryOper() 
{
	pair<int, string> lexeme = lookForward(1);
	bool operationFlag = false;

	if (lexeme.first == typePlus || lexeme.first == typeMinus || lexeme.first == typePlusPlus || lexeme.first == typeMinusMinus) 
	{
		lexeme = Scan();
		operationFlag = true;
	}

	TYPE_DATA type = SimpleOper();
	lexeme = lookForward(1);

	if (lexeme.first == typePlus || lexeme.first == typeMinus || lexeme.first == typePlusPlus || lexeme.first == typeMinusMinus)
	{
		lexeme = Scan();
		operationFlag = true;
	}

	if (operationFlag && type == TYPE_UNKNOWN)
		tree->PrintError("TypeError", lexeme);

	return type;
}

TYPE_DATA SyntaxAnalyzer::SimpleOper() 
{
	pair<int, string> lexeme = lookForward(1);

	if (lexeme.first == typeLeftBracket) {
		lexeme = Scan();
		TYPE_DATA type = Calculator();
		lexeme = Scan();

		if (lexeme.first != typeRightBracket)
			PrintError("Waiting )", lexeme);

		return type;
	}
	else if (lexeme.first == constInt || lexeme.first == constDouble)
	{
		lexeme = Scan();
		if (lexeme.first == constInt)
			return TYPE_SHORT;
		else return TYPE_DOUBLE;
	}
	else if (lexeme.first == typeID) {
		lexeme = Scan();

		SemanticTree* node = tree->findUp(lexeme.second);

		if (node == NULL)
			tree->PrintError("Waiting ID", lexeme);
		TYPE_OBJECT foundedNodeType = node->getSelfObjectType();
		int init = node->isSelfInit();
		if (foundedNodeType == OBJ_CLASS_OBJ) node = node->getClassPointer();

		lexeme = lookForward(1);
		while (lexeme.first == typePoint) {
			lexeme = Scan();
			lexeme = Scan();
			if (!init && foundedNodeType == OBJ_CLASS_OBJ)
				tree->PrintError("Var is not initialized", lexeme);
			if (lexeme.first != typeID)
				tree->PrintError("Waiting ID", lexeme);

			node = node->findRightLeft(lexeme.second);

			if (node == NULL)
				tree->PrintError("ID is not found", lexeme);
			foundedNodeType = node->getSelfObjectType();
			init = node->isSelfInit();
			if (foundedNodeType == OBJ_CLASS_OBJ) {
				node = node->getClassPointer();
			}

			lexeme = lookForward(1);
		}

		if (lexeme.first == typeLeftBracket) {
			lexeme = Scan();
			lexeme = Scan();
			if (lexeme.first != typeRightBracket)
				PrintError("Waiting )", lexeme);

			if (foundedNodeType == OBJ_CLASS)
				return TYPE_UNKNOWN;
			else if (foundedNodeType == OBJ_FUNC)
				return node->getSelfDataType();
			else
				tree->PrintError("Restricted call", lexeme);
		}
		else
			if (!init && (foundedNodeType == OBJ_VAR || foundedNodeType == OBJ_CLASS_OBJ))
				tree->PrintError("Var is not initialized", lexeme);

		return node->getSelfDataType();
	}
	else
		PrintError("Waiting simple operation", lexeme);
}

TYPE_DATA SyntaxAnalyzer::Calculator() 
{
	TYPE_DATA type = Sum();

	pair<int, string> lexeme = lookForward(1);

	while (lexeme.first == typeLess || lexeme.first == typeLessOrEq || lexeme.first == typeMore || lexeme.first == typeMoreOrEq || lexeme.first == typeEq || lexeme.first == typeUnEq) 
	{
		lexeme = Scan();
		int operation = lexeme.first;
		TYPE_DATA type2 = Sum();
		lexeme = lookForward(1);

		if (type == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		if (type2 == TYPE_UNKNOWN)
			tree->PrintError("TypeError", lexeme);
		type = tree->getTypeResult(type, type2, operation);
		if (type == TYPE_UNKNOWN) 
			tree->PrintError("TypeError", lexeme);
	}

	return type;
}