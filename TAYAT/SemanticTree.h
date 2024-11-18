#pragma once
#ifndef __SEMANTTREE
#define __SEMANTTREE

#include "Lexeme.h"
#include "Scanner.h"

enum TYPE_DATA {
	TYPE_UNKNOWN = 0,
	TYPE_SHORT = 1,
	TYPE_INTEGER = 2,
	TYPE_LONG = 3,
	TYPE_DOUBLE = 4,
};

enum TYPE_OBJECT {
	OBJ_VAR = 1,
	OBJ_CONST = 2,
	OBJ_CLASS = 3,
	OBJ_CLASS_OBJ = 4,
	OBJ_FUNC = 5,
};

class SemanticTree;

struct Node {
	string id;
	TYPE_OBJECT objectType;
	TYPE_DATA dataType;
	int flagInit;
	SemanticTree* pointer; // ”казатель на объект
	SemanticTree* classPointer; // ”казатель на класс
	void setInit() {
		flagInit = 1;
	};
};

class SemanticTree
{
protected:
	Node* node;
	SemanticTree* up;
	SemanticTree* left;
	SemanticTree* right;
public:
	SemanticTree();
	SemanticTree(SemanticTree*, SemanticTree*, SemanticTree*, Node*);
	~SemanticTree();

	SemanticTree* findMethod(const TypeLex& lex);
	void PrintError(string errorMessage, string lexeme);

	void setLeft(Node*);
	void setRight(Node*);

	SemanticTree* getLeft();
	SemanticTree* getRight();
	SemanticTree* getNode();
	TYPE_OBJECT getSelfObjectType();
	SemanticTree* getClassPointer();
	TYPE_DATA getSelfDataType();
	string getSelfId();
	bool isSelfInit();
	
	SemanticTree* findUp(SemanticTree*, string);
	SemanticTree* findUp(string);
	SemanticTree* findUp(SemanticTree*, string, TYPE_OBJECT);
	SemanticTree* findUp(string, TYPE_OBJECT);
	SemanticTree* findUp(SemanticTree*, TYPE_OBJECT);
	SemanticTree* findUp(TYPE_OBJECT);
	SemanticTree* findUpOneLevel(SemanticTree*, string);
	SemanticTree* findRightLeft(SemanticTree* from, string id);
	SemanticTree* findRightLeft(string id);
	SemanticTree* findLeft(SemanticTree* from, string id);
	SemanticTree* findLeft(string id);

	TYPE_DATA getDataType(int);

	TYPE_DATA getTypeResult(TYPE_DATA, TYPE_DATA, int);

	void print(int);
	void print();
	bool isDoublicateId(SemanticTree*, string);
	bool isAllowChangeId(string);
	bool isMainExists();
	bool isInFor();
	bool canBeAdded(TYPE_DATA type1, TYPE_DATA type2);
	bool canBeAdded(int type1, int type2);

};

#endif