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
	OBJ_UNKNOWN = 0,
	OBJ_VAR = 1,
	OBJ_CONST = 2,
	OBJ_CLASS = 3,
	OBJ_CLASS_OBJ = 4,
	OBJ_FUNC = 5,
};

class SemanticTree;

union TYPE_VALUE {
	int dataInt;
	long dataLong;
	short dataShort;
	double dataDouble = 0;
};

struct TData {
	TYPE_DATA dataType;
	TYPE_VALUE value;
	TData() {
		dataType = TYPE_UNKNOWN;
		value.dataInt = 0;
		value.dataLong = 0;
		value.dataShort = 0;
		value.dataDouble = 0;
	}
};



struct Node {
	string id;
	TypeLex lex;
	TYPE_OBJECT objectType;
	TData dataType;
	int flagInit;
	SemanticTree* pointer; // Указатель на объект
	SemanticTree* classPointer; // Указатель на класс
	void setInit() {
		flagInit = 1;
	};
	// Метод для установки значения
	void setValue(const TYPE_VALUE& newValue) {
		switch (dataType.dataType) {
		case TYPE_INTEGER:
			try {
				dataType.value.dataInt = newValue.dataDouble;
			}
			catch (const std::invalid_argument&) {
				throw std::runtime_error("Invalid value for integer conversion.");
			}
			catch (const std::out_of_range&) {
				throw std::runtime_error("Integer value out of range.");
			}
			break;
		case TYPE_LONG: {
			try {
				dataType.value.dataLong = newValue.dataDouble;
			}
			catch (const std::invalid_argument&) {
				throw std::runtime_error("Invalid value for long conversion.");
			}
			catch (const std::out_of_range&) {
				throw std::runtime_error("Long value out of range.");
			}
			break;
		}
		case TYPE_SHORT: {
			try {
				dataType.value.dataShort = newValue.dataDouble;
			}
			catch (const std::invalid_argument&) {
				throw std::runtime_error("Invalid value for short conversion.");
			}
			catch (const std::out_of_range&) {
				throw std::runtime_error("Short value out of range.");
			}
			break;
		}
		case TYPE_DOUBLE: {
			try {
				dataType.value.dataDouble = newValue.dataDouble;
			}
			catch (const std::invalid_argument&) {
				throw std::runtime_error("Invalid value for double conversion.");
			}
			catch (const std::out_of_range&) {
				throw std::runtime_error("Double value out of range.");
			}
			break;
		}
		default:
			// Обработка неизвестного типа
			//throw std::runtime_error("Unknown data type.");
			break;
		}
	};
};

class SemanticTree
{
protected:
	Scanner* scaner;
	Node* node;
	SemanticTree* up;
	SemanticTree* left;
	SemanticTree* right;
public:
	SemanticTree* current;
	SemanticTree();
	SemanticTree(Scanner*);
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
	//SemanticTree* semantic_include(TypeLex lex, TYPE_OBJECT object, TYPE_DATA type);
	TYPE_DATA getDataType(int);
	void setDataType(TYPE_DATA, TYPE_VALUE);

	TYPE_DATA getTypeResult(TYPE_DATA, TYPE_DATA, int);
	TYPE_VALUE getData();
	void print(int);
	void print();
	bool isDoublicateId(SemanticTree*, string);
	bool isAllowChangeId(string);
	bool isMainExists();
	bool isInFor();
	bool canBeAdded(TYPE_DATA type1, TYPE_DATA type2);
	bool canBeAdded(int type1, int type2);
	int is_exists(SemanticTree* tree, TypeLex lex);
	bool setValue(const string& id, const TYPE_VALUE& val);

	void setCurrent(SemanticTree* tree);

	SemanticTree* getCurrent();

};

#endif