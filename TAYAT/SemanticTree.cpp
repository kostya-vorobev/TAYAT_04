#include "SemanticTree.h"

SemanticTree::SemanticTree()
{
	up = NULL;
	left = NULL;
	right = NULL;
	node = NULL;
}

SemanticTree::SemanticTree(SemanticTree* up, SemanticTree* left, SemanticTree* right, Node* node)
{
	this->up = up;
	this->left = left;
	this->right = right;
	this->node = new Node();
	if (node != NULL) memcpy(this->node, node, sizeof(Node));
}

SemanticTree::~SemanticTree()
{
	if (left != nullptr) delete left;
	if (right != nullptr) delete right;
	if (node != nullptr) delete node;
}

void SemanticTree::PrintError(string errorMessage,  string lexeme)
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
		std::cout << "right: " << right->node->id << " " ;
		right->print(); // рекурсивный вызов для правого дочернего узла
	}
	std::cout << std::endl;
}

void SemanticTree::print(int level = 0) {
	// Увеличиваем отступ для отображения иерархии
	std::string indent(level * 2, ' ');

	if (node != NULL ) {
		if (node->id == "")
			std::cout << "Node: " << "\n";
		else
		std::cout <<  "Node: " << node->id << "\n";
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
	return node->pointer;
}

TYPE_DATA SemanticTree::getSelfDataType()
{
	return node->dataType;
}

string SemanticTree::getSelfId()
{
	return node->id;
}

bool SemanticTree::isSelfInit()
{
	return node->flagInit == 1;
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