#include "SemanticTree.h"

SemanticTree::SemanticTree()
{
	up = NULL;
	left = NULL;
	right = NULL;
	this->node = new Node();
	if (node != NULL) {
		memcpy(this->node, node, sizeof(Node));
	}
}

SemanticTree::SemanticTree(Scanner* scanner)
{
	scaner = scanner;
	up = NULL;
	left = NULL;
	right = NULL;
	this->node = new Node();
	if (node != NULL) {
		memcpy(this->node, node, sizeof(Node));

	}
}

SemanticTree::SemanticTree(SemanticTree* up, SemanticTree* left, SemanticTree* right, Node* node)
{
	this->up = up;
	this->left = left;
	this->right = right;
	this->node = new Node();

	if (node != NULL) {
		memcpy(this->node, node, sizeof(Node));
		node->dataType = TData();
	}
}

SemanticTree::~SemanticTree()
{
	if (left != nullptr) delete left;
	if (right != nullptr) delete right;
	if (node != nullptr) delete node;
}

void SemanticTree::PrintError(string errorMessage, string lexeme)
{
	cout << "Semant Error. " << errorMessage << " Find: " << lexeme << endl;
	exit(1);
}

void SemanticTree::setLeft(Node* node)
{
	this->left = new SemanticTree(this, NULL, NULL, node);
}

SemanticTree* SemanticTree::findMethod(const TypeLex& lex) {
	SemanticTree* currentNode = this;

	// Перебираем узлы в дереве
	while (currentNode != nullptr) {
		if (currentNode->node != nullptr &&
			currentNode->node->id == lex &&
			currentNode->node->objectType == OBJ_FUNC) { // Проверка на тип объекта
			return currentNode; // Если нашли метод, вернем его
		}
		currentNode = currentNode->getRight(); // Продвижение к следующему узлу
	}
	return nullptr; // Метод не найден
}

void SemanticTree::setRight(Node* node)
{
	this->right = new SemanticTree(this, NULL, NULL, node);
}

SemanticTree* SemanticTree::getLeft()
{
	return this->left;
}

SemanticTree* SemanticTree::getRight()
{
	return this->right;
}
/*
SemanticTree* SemanticTree::getNode()
{
	return this->node->classPointer;
}*/
Node* SemanticTree::getNode()
{
	return this->node;
}

SemanticTree* SemanticTree::findUp(SemanticTree* from, string id)
{
	SemanticTree* current = from;

	while (current != NULL) {
		if (current->node != NULL) {
			if (id.compare(current->node->id) == 0) break;
		}
		current = current->up;
	}

	return current;
}

SemanticTree* SemanticTree::findUp(string id)
{
	return findUp(this, id);
}

SemanticTree* SemanticTree::findUp(SemanticTree* from, string id, TYPE_OBJECT type)
{
	SemanticTree* current = from;

	while (current != NULL) {
		if (current->node != NULL) {
			if (id.compare(current->node->id) == 0 && type == current->node->objectType) break;
		}

		current = current->up;
	}

	return current;
}

SemanticTree* SemanticTree::findUp(string id, TYPE_OBJECT type)
{
	return findUp(this, id, type);
}

SemanticTree* SemanticTree::findUp(SemanticTree* from, TYPE_OBJECT type)
{
	SemanticTree* current = from;

	while (current != NULL) {
		if (current->node != NULL) {
			if (type == current->node->objectType) break;
		}

		current = current->up;
	}

	return current;
}

SemanticTree* SemanticTree::findUp(TYPE_OBJECT type)
{
	return findUp(this, type);
}

SemanticTree* SemanticTree::findUpOneLevel(SemanticTree* from, string id)
{
	SemanticTree* current = from;

	while (current != NULL && current->up != NULL && current->up->right != current)
	{
		if (id.compare(current->node->id) == 0) {
			return current;
		}
		current = current->up;
	}
	return NULL;
}

SemanticTree* SemanticTree::findRightLeft(SemanticTree* from, string id)
{
	SemanticTree* curNode = from->right;
	while (curNode != NULL) {
		if (curNode->node != NULL) {
			if (id.compare(curNode->node->id) == 0) break;
		}

		curNode = curNode->left;
	}
	return curNode;
}

SemanticTree* SemanticTree::findRightLeft(string id)
{
	return findRightLeft(this, id);
}

SemanticTree* SemanticTree::findLeft(SemanticTree* from, string id)
{
	SemanticTree* current = from;

	while (current != NULL)
	{
		if (current->node != NULL)
			if (id.compare(current->node->id) == 0)
				break;

		current = current->left;
	}

	return current;
}

SemanticTree* SemanticTree::findLeft(string id)
{
	return findLeft(this, id);
}

TYPE_DATA SemanticTree::getDataType(int type)
{
	if (type == typeInt) return TYPE_INTEGER;
	if (type == typeShort) return TYPE_SHORT;
	if (type == typeLong) return TYPE_LONG;
	if (type == typeDouble) return TYPE_DOUBLE;
	return TYPE_UNKNOWN;
}

TYPE_DATA SemanticTree::getTypeResult(TYPE_DATA type1, TYPE_DATA type2, int operation) {
	if (operation >= typeUnEq && operation <= typeMore) return TYPE_SHORT;
	if (operation == typeMod && (type1 == TYPE_DOUBLE || type2 == TYPE_DOUBLE)) return TYPE_UNKNOWN;

	if (type1 >= type2) return type1;

	return type2;
}

void SemanticTree::print() {
	if (node != NULL) {
		std::cout << "node: " << node->id << " ";
	}

	if (left != NULL) {
		std::cout << "left: " << left->node->id << " ";
		left->print(); // рекурсивный вызов для левого дочернего узла
	}

	if (right != NULL) {
		std::cout << "right: " << right->node->id << " ";
		right->print(); // рекурсивный вызов для правого дочернего узла
	}
	std::cout << std::endl;
}

void SemanticTree::print(int level = 0) {
	// Увеличиваем отступ для отображения иерархии
	std::string indent(level * 2, ' ');

	if (node != NULL) {
		std::cout << "Node: " << node->id << " ("
			<< node->dataType.dataType << ") ";

		// Проверка типа данных и вывод соответствующего значения
		if ((node->objectType == OBJ_VAR || node->objectType == OBJ_CONST) && node->flagInit == 1)
			switch (node->dataType.dataType) {
			case TYPE_INTEGER:
				std::cout << "Value: " << node->dataType.value.dataInt << "\n";
				break;
			case TYPE_LONG:
				std::cout << "Value: " << node->dataType.value.dataLong << "\n";
				break;
			case TYPE_SHORT:
				std::cout << "Value: " << node->dataType.value.dataShort << "\n";
				break;
			case TYPE_DOUBLE:
				std::cout << "Value: " << node->dataType.value.dataDouble << "\n";
				break;
			default:
				std::cout << "Value: Unknown data type\n";
				break;
			}
		else
		{
			std::cout << "\n";
		}
	}
	else
	{
		std::cout << indent << "Node: \n";
	}
	if (right != NULL) {

		std::cout << indent << "Right -> ";

		right->print(level + 1); // рекурсивный вызов для правого дочернего узла

	}
	if (left != NULL) {
		//if (left->node->id != "") {
		std::cout << indent << "Left -> ";
		//}
		left->print(level + 1); // рекурсивный вызов для левого дочернего узла

	}




}


bool SemanticTree::isDoublicateId(SemanticTree* addr, string id) {
	if (findUpOneLevel(addr, id) == NULL) return false;
	return true;
}

bool SemanticTree::isAllowChangeId(string id) {
	SemanticTree* node = findUp(id);

	if (node->node->objectType != OBJ_VAR && node->node->objectType != OBJ_CLASS_OBJ) return false;

	return true;
}

bool SemanticTree::isMainExists() {
	SemanticTree* root = this;

	while (root->up != NULL) root = root->up;

	SemanticTree* main = root->findLeft("main");

	if (main == NULL) return false;
	return true;
}

TYPE_OBJECT SemanticTree::getSelfObjectType()
{
	return node->objectType;
}

SemanticTree* SemanticTree::getClassPointer()
{
	return this->node->classPointer;
}

SemanticTree* SemanticTree::getClassNode()
{
	return this->node->classNode;
}

TYPE_DATA SemanticTree::getSelfDataType()
{
	return node->dataType.dataType;
}

string SemanticTree::getSelfId()
{
	return node->id;
}

bool SemanticTree::isSelfInit()
{
	return node->flagInit;
}

bool SemanticTree::canBeAdded(TYPE_DATA type1, TYPE_DATA type2) {
	// Для примера: только числовые типы можно складывать
	if ((type1 == typeInt || type1 == typeDouble) &&
		(type2 == typeInt || type2 == typeDouble)) {
		return true;
	}
	return false;
}

bool SemanticTree::canBeAdded(int type1, int type2) {
	// Для примера: только числовые типы можно складывать
	if ((type1 == typeInt || type1 == typeDouble) &&
		(type2 == typeInt || type2 == typeDouble)) {
		return true;
	}
	return false;
}

void  SemanticTree::setDataType(TYPE_DATA newType, TYPE_VALUE newValue) {
	if (node != nullptr) {
		node->dataType.dataType = newType;
		node->dataType.value = newValue; // Установка нового значения
	}
}

int SemanticTree::is_exists(SemanticTree* tree, TypeLex lex) {
	if (findUpOneLevel(tree, lex) == NULL)
		return 0;
	return 1;
}

void SemanticTree::setCurrent(SemanticTree* tree) {
	current = tree;
}

SemanticTree* SemanticTree::getCurrent() {
	return(current);
}

bool SemanticTree::setValue(const string& id, const TYPE_VALUE& val) {
	SemanticTree* node = findUp(id);  // Метод поиска узла по идентификатору
	if (node == nullptr) {
		return false; // Узел не найден
	}
	//node->node->setValue(val); // Установка нового значения
	switch (node->node->dataType.dataType) {
	case TYPE_INTEGER:
		node->node->dataType.value.dataInt = (int)val.dataDouble;
		break;
	case TYPE_LONG: {
		node->node->dataType.value.dataLong = (long)val.dataDouble;
		break;
	}
	case TYPE_SHORT: {
		node->node->dataType.value.dataShort = (short)val.dataDouble;
		break;
	}
	case TYPE_DOUBLE: {
		node->node->dataType.value.dataDouble = val.dataDouble;
		break;
	}
	default:
		// Обработка неизвестного типа
		//throw std::runtime_error("Unknown data type.");
		break;
	}
	return true; // Успешно обновлено
}

TData* SemanticTree::getData() {
	TData* val = new TData;
	switch (this->node->dataType.dataType) {
	case TYPE_INTEGER:
		val->dataType = TYPE_INTEGER;
		val->value.dataDouble = this->node->dataType.value.dataInt;
		break;
	case TYPE_LONG: {
		val->dataType = TYPE_LONG;
		val->value.dataDouble = this->node->dataType.value.dataLong;
		break;
	}
	case TYPE_SHORT: {
		val->dataType = TYPE_SHORT;
		val->value.dataDouble = this->node->dataType.value.dataShort;
		break;
	}
	case TYPE_DOUBLE: {
		val->dataType = TYPE_DOUBLE;
		val->value.dataDouble = this->node->dataType.value.dataDouble;
		break;
	}
	default:
		// Обработка неизвестного типа
		//throw std::runtime_error("Unknown data type.");
		break;
	}
	return val;
}
void SemanticTree::setInit() {
	this->node->flagInit = 1;
}

SemanticTree* SemanticTree::copyTree() {
	SemanticTree* newTree = new SemanticTree();
	newTree->node = new Node();

	// Копирование данных из текущего узла
	*newTree->node = *this->node; // Копируем данные узла, включая classNode

	if (this->node->classNode != nullptr) {
		newTree->node->classNode = this->node->classNode->copyTree(); // Копируем classNode
	}

	if (this->left) {
		newTree->left = this->left->copyTree(); // Рекурсивное копирование левого поддерева
		newTree->left->up = newTree; // Устанавливаем родителя для нового дерева
	}

	if (this->right) {
		newTree->right = this->right->copyTree(); // Рекурсивное копирование правого поддерева
		newTree->right->up = newTree; // Устанавливаем родителя для нового дерева
	}

	return newTree;
}

bool SemanticTree::flag_interp = false;