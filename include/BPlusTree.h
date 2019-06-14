#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

#include <functional>
#include <list>
#include <memory>
#include <utility>
#include <iostream>
#include "Exception.h"
#define TEMP_DEF template <typename T, int M>
#define TEMP_NODE INode<T, M>
#define TEMP_TREE BPlusTree<T, M>
#define TEMP_POSITION LeafPosition<T, M>

TEMP_DEF
struct INode {
	int id;
	bool isLeaf;
	int contentSize;
	T value[M];
	int pointers[M + 1]; // If isLeaf = true, then pointers[M] points to the next leaf.
};

TEMP_DEF
struct LeafPosition {
	TEMP_NODE* node;
	int position;
	LeafPosition(TEMP_NODE* node, int position)
		: node(node), position(position) {}
};

TEMP_DEF
class BPlusTree {
public:
	BPlusTree(TEMP_NODE* root, std::function<TEMP_NODE*(int)> getChildFunc,
		std::function<void(TEMP_NODE*)> saveNodeFunc,
		std::function<void(TEMP_NODE*)> deleteNodeFunc,
		std::function<TEMP_NODE*(void)> createNodeFunc)
		: _root(root), _getChildFunc(getChildFunc), _saveNodeFunc(saveNodeFunc),
		_deleteNodeFunc(deleteNodeFunc), _createNodeFunc(createNodeFunc) {
	}
	std::shared_ptr<TEMP_POSITION> find(T value);
	std::shared_ptr<TEMP_POSITION> findExtreme(bool isMax);
	void insert(T value, int pointer);
	bool del(TEMP_NODE& value);
	TEMP_NODE* next(TEMP_NODE* leafNode) const;
#ifdef _DEBUG
	void printTree() const;
#endif // _DEBUG

private:
	TEMP_NODE* _root;
	std::function<TEMP_NODE*(int)> _getChildFunc;
	std::function<void(TEMP_NODE*)> _saveNodeFunc;
	std::function<void(TEMP_NODE*)> _deleteNodeFunc;
	std::function<TEMP_NODE*(void)> _createNodeFunc;
	std::list<std::shared_ptr<TEMP_POSITION>> _stack;
	void _insertArray(TEMP_NODE* node, T value, int position);
	int _findPos(T values[], T value, int size);
	void _insert(TEMP_NODE* node, T value, int pointer);
};

/* Implementation */

TEMP_DEF
TEMP_NODE* TEMP_TREE::next(TEMP_NODE* leafNode) const
{
	if (!leafNode->isLeaf) {
		throw SQLException("Attemp to iterate a non-leaf node in B+ tree!");
	}
	return _getChildFunc(leafNode->pointers[M]);
}

TEMP_DEF
std::shared_ptr<TEMP_POSITION> TEMP_TREE::find(T value)
{
	_stack.clear();
	TEMP_NODE* current = _root;
	while (!current->isLeaf) {
		int i = _findPos(current->value, value, current->contentSize);
		_stack.push_back(std::make_shared<TEMP_POSITION>(current, i));
		current = _getChildFunc(current->pointers[i]);
	}
	int i = _findPos(current->value, value, current->contentSize);
	return std::make_shared<TEMP_POSITION>(current, i);
}

TEMP_DEF
std::shared_ptr<TEMP_POSITION> TEMP_TREE::findExtreme(bool isMax)
{
	TEMP_NODE* current = _root;
	while (!current->isLeaf) {
		current = _getChildFunc(current->pointers[isMax ? current->contentSize : 0]);
	}
	return std::make_shared<TEMP_POSITION>(current, isMax ? current->contentSize - 1 : 0);
}

TEMP_DEF
void TEMP_TREE::insert(T value, int pointer)
{
	auto result = find(value);
	_insert(result->node, value, pointer);
}

TEMP_DEF
void TEMP_TREE::_insert(TEMP_NODE* node, T value, int pointer)
{
	if (node->contentSize < M) {
		int rightP = node->pointers[node->contentSize];
		int i;
		for (i = node->contentSize; i >= 1 && node->value[i - 1] >= value; i--) {
			node->value[i] = node->value[i - 1];
			node->pointers[i] = node->pointers[i - 1];
		}
		node->value[i] = value;
		node->pointers[i] = pointer;
		node->contentSize++;
		if (!node->isLeaf) {
			node->pointers[node->contentSize] = rightP;
		}
		_saveNodeFunc(node);
	}
	else {
		TEMP_NODE* newNode = _createNodeFunc();
		if (newNode->isLeaf = node->isLeaf) {
			newNode->pointers[M] = node->id;
		}
		T container[M + 1];
		int pointersContainer[M + 1];
		int i, j;
		for (i = 0, j = 0; i < M; i++, j++) {
			if (node->value[i] >= value && j == i) {
				container[j] = value;
				pointersContainer[j] = pointer;
				j++;
			}
			container[j] = node->value[i];
			pointersContainer[j] = node->pointers[i];
		}
		if (j == i) {
			container[j] = value;
			pointersContainer[j] = pointer;
		}
		int leftSize = (M + 1) / 2;
		int rightSize = M + 1 - leftSize;
		newNode->contentSize = leftSize - !node->isLeaf;
		node->contentSize = rightSize;
		for (i = 0; i < leftSize; i++) {
			if (i < newNode->contentSize) {
				newNode->value[i] = container[i];
			}
			newNode->pointers[i] = pointersContainer[i];
		}
		for (i = 0; i < rightSize; i++) {
			node->value[i] = container[i + leftSize];
			node->pointers[i] = pointersContainer[i + leftSize];
		}
		node->pointers[rightSize] = node->pointers[M];
		_saveNodeFunc(newNode);
		_saveNodeFunc(node);
		if (_stack.empty()) {
			_root = _createNodeFunc();
			_root->isLeaf = false;
			_root->contentSize = 1;
			_root->value[0] = container[leftSize - 1];
			_root->pointers[0] = newNode->id;
			_root->pointers[1] = node->id;
			_saveNodeFunc(_root);
		}
		else {
			auto point = _stack.back();
			_stack.pop_back();
			_insert(point->node, container[leftSize - 1], newNode->id);
		}
	}
}

TEMP_DEF
bool TEMP_TREE::del(TEMP_NODE& value)
{
	// TODO
	throw std::exception()
}

TEMP_DEF
int TEMP_TREE::_findPos(T values[], T value, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		if (values[i] >= value) {
			break;
		}
	}
	return i;
}

TEMP_DEF
void TEMP_TREE::_insertArray(TEMP_NODE* node, T value, int position)
{
	for (int i = node->contentSize; i > position; i--) {
		node->value[i] = node->value[i - 1];
	}
	node->value[i] = value;
	node->contentSize++;
}

#ifdef _DEBUG
TEMP_DEF
void TEMP_TREE::printTree() const
{
	std::list<std::pair<int, int>> queue;
	queue.push_back(std::make_pair(0, _root->id));
	int preLevel = -1;
	std::cout << "B+ Tree: ";
	while (!queue.empty()) {
		std::pair<int, int> p = queue.front();
		queue.pop_front();
		if (p.second <= 0) {
			continue;
		}
		if (preLevel != p.first) {
			preLevel = p.first;
			std::cout << std::endl << preLevel << ": ";
		}
		TEMP_NODE* node = _getChildFunc(p.second);
		std::cout << "[" << node->id << "]";
		int i;
		for (i = 0; i < node->contentSize; i++) {
			if (!node->isLeaf) {
				std::cout << "(" << node->pointers[i] << ")";
				queue.push_back(std::make_pair(p.first + 1, node->pointers[i]));
			}
			std::cout << node->value[i];
			if (i != node->contentSize - 1) {
				std::cout << ",";
			}
		}
		if (!node->isLeaf) {
			std::cout << "(" << node->pointers[i] << ")";
			queue.push_back(std::make_pair(p.first + 1, node->pointers[i]));
		}
		else {
			std::cout << "(" << node->pointers[M] << ")";
		}
		std::cout << " | ";
	}
}
#endif //_DEBUG

#endif // _B_PLUS_TREE_H_
