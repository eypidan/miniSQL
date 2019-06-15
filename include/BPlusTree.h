#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

#include <functional>
#include <list>
#include <memory>
#include <utility>
#include <iostream>
#include "Exception.h"
#define TEMP_DEF template <typename T, int NodeSize>
#define TEMP_NODE INode<T, NodeSize>
#define TEMP_TREE BPlusTree<T, NodeSize>
#define TEMP_POSITION LeafPosition<T, NodeSize>
#define M ((NodeSize - 4 * sizeof(int) - sizeof(bool)) / (sizeof(T) + sizeof(int)))

TEMP_DEF
struct INode {
	int id;
	bool isLeaf;
	int contentSize;
	T value[M];
	int pointers[M + 1]; // If isLeaf = true, then pointers[M] points to the next leaf.
	int lastPoint; // If isLeaf = true, then lastPoint points to the last leaf.
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
		std::function<void(TEMP_NODE*)> modifyNodeFunc,
		std::function<void(TEMP_NODE*)> deleteNodeFunc,
		std::function<TEMP_NODE*(void)> createNodeFunc)
		: _root(root), _getChildFunc(getChildFunc), _modifyNodeFunc(modifyNodeFunc),
		_deleteNodeFunc(deleteNodeFunc), _createNodeFunc(createNodeFunc) {
	}
	TEMP_POSITION* find(T& value);
	TEMP_POSITION* findExtreme(bool isMax);
	void insert(T& value, int pointer);
	bool del(T& value);
	TEMP_NODE* next(TEMP_NODE* leafNode) const;
	int getM() const { return M; };
#ifdef _DEBUG
	void printTree() const;
#endif // _DEBUG

private:
	TEMP_NODE* _root;
	std::function<TEMP_NODE*(int)> _getChildFunc;
	std::function<void(TEMP_NODE*)> _modifyNodeFunc;
	std::function<void(TEMP_NODE*)> _deleteNodeFunc;
	std::function<TEMP_NODE*(void)> _createNodeFunc;
	std::list<std::shared_ptr<TEMP_POSITION>> _stack;
	int _findPos(T values[], T value, int size);
	void _delInternal();
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
TEMP_POSITION* TEMP_TREE::find(T& value)
{
	_stack.clear();
	TEMP_NODE* current = _root;
	while (!current->isLeaf) {
		int i = _findPos(current->value, value, current->contentSize);
		_stack.push_back(std::make_shared<TEMP_POSITION>(current, i));
		current = _getChildFunc(current->pointers[i]);
	}
	int i = _findPos(current->value, value, current->contentSize) - 1;
	return new TEMP_POSITION(current, i);
}

TEMP_DEF
TEMP_POSITION* TEMP_TREE::findExtreme(bool isMax)
{
	TEMP_NODE* current = _root;
	while (!current->isLeaf) {
		current = _getChildFunc(current->pointers[isMax ? current->contentSize : 0]);
	}
	return new TEMP_POSITION(current, isMax ? current->contentSize - 1 : 0);
}

TEMP_DEF
void TEMP_TREE::insert(T& value, int pointer)
{
	auto result = find(value);
	TEMP_NODE* node = result->node;
	while (true) {
		if (node->contentSize < M) {
			int rightP = node->pointers[node->contentSize];
			int i;
			for (i = node->contentSize; i >= 1 && node->value[i - 1] > value; i--) {
				node->value[i] = node->value[i - 1];
				node->pointers[i] = node->pointers[i - 1];
			}
			node->value[i] = value;
			node->pointers[i] = pointer;
			node->contentSize++;
			if (!node->isLeaf) {
				node->pointers[node->contentSize] = rightP;
			}
			_modifyNodeFunc(node);
			break;
		}
		else {
			TEMP_NODE* newNode = _createNodeFunc();
			if (newNode->isLeaf = node->isLeaf) {
				newNode->pointers[M] = node->id;
				newNode->lastPoint = node->lastPoint;
				if (node->lastPoint > 0) {
					TEMP_NODE* lastNode = _getChildFunc(node->lastPoint);
					lastNode->pointers[M] = newNode->id;
					newNode->lastPoint = lastNode->id;
					_modifyNodeFunc(lastNode);
				}
				node->lastPoint = newNode->id;
			}
			T container[M + 1];
			int pointersContainer[M + 1];
			int i, j;
			for (i = 0, j = 0; i < M; i++, j++) {
				if (node->value[i] > value && j == i) {
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
			T middle = container[leftSize];
			newNode->contentSize = leftSize;
			node->contentSize = rightSize - !node->isLeaf;
			for (i = 0; i < newNode->contentSize; i++) {
				newNode->value[i] = container[i];
				newNode->pointers[i] = pointersContainer[i];
			}
			newNode->pointers[i] = pointersContainer[i];
			for (i = 0, j = leftSize + !node->isLeaf; i < node->contentSize; i++, j++) {
				node->value[i] = container[j];
				node->pointers[i] = pointersContainer[j];
			}
			node->pointers[node->contentSize] = node->pointers[M];
			_modifyNodeFunc(newNode);
			_modifyNodeFunc(node);
			if (_stack.empty()) {
				_root = _createNodeFunc();
				_root->isLeaf = false;
				_root->contentSize = 1;
				_root->value[0] = middle;
				_root->pointers[0] = newNode->id;
				_root->pointers[1] = node->id;
				_modifyNodeFunc(_root);
				break;
			}
			else {
				auto point = _stack.back();
				_stack.pop_back();
				node = point->node;
				value = middle;
				pointer = newNode->id;
			}
		}
	}
}

TEMP_DEF
bool TEMP_TREE::del(T& value)
{
	auto result = find(value);
	TEMP_NODE* node = result->node;
	if (result->position >= node->contentSize) {
		std::cout << value << " not found!" << std::endl;
		throw SQLException("Value not found in index");
	}
	// delete array
	for (int i = result->position; i < node->contentSize - 1; i++) {
		node->value[i] = node->value[i + 1];
		node->pointers[i] = node->pointers[i + 1];
	}
	node->contentSize--;
	_modifyNodeFunc(node);
	// reach root
	if (node->id == _root->id) {
		return true;
	}
	// Fetch parent
	auto parentResult = _stack.back();
	TEMP_NODE* parentNode = parentResult->node;
	int parentPos = parentResult->position;
	// No need to borrow or merge
	if (node->contentSize >= (M + 1) / 2) {
		if (parentPos != 0) {
			parentNode->value[parentPos - 1] = node->value[0];
			_modifyNodeFunc(parentNode);
		}
		return true;
	}

	if (parentResult->position != 0) {
		TEMP_NODE* sibling = _getChildFunc(parentNode->pointers[parentPos - 1]);
		_modifyNodeFunc(sibling);
		if (sibling->contentSize > (M + 1) / 2) { // borrow
			for (int i = node->contentSize; i >= 1; i--) {
				node->value[i] = node->value[i - 1];
				node->pointers[i] = node->pointers[i - 1];
			}
			node->value[0] = sibling->value[sibling->contentSize - 1];
			node->pointers[0] = sibling->pointers[sibling->contentSize - 1];
			parentNode->value[parentPos - 1] = node->value[0];
			sibling->contentSize--;
			node->contentSize++;
		}
		else { // merge
			for (int i = 0; i < node->contentSize; i++) {
				sibling->value[sibling->contentSize + i] = node->value[i];
				sibling->pointers[sibling->contentSize + i] = node->pointers[i];
			}
			sibling->contentSize += node->contentSize;
			sibling->pointers[M] = node->pointers[M];
			_deleteNodeFunc(node);
			_delInternal();
		}
	}
	else {
		TEMP_NODE* sibling = _getChildFunc(parentNode->pointers[parentPos + 1]);
		_modifyNodeFunc(sibling);
		if (sibling->contentSize > (M + 1) / 2) { // borrow
			node->value[node->contentSize] = sibling->value[0];
			node->pointers[node->contentSize] = sibling->pointers[0];
			parentNode->value[parentPos] = sibling->value[1];
			for (int i = 0; i < sibling->contentSize - 1; i++) {
				sibling->value[i] = sibling->value[i + 1];
				sibling->pointers[i] = sibling->pointers[i + 1];
			}
			node->contentSize++;
			sibling->contentSize--;
		}
		else { // merge
			for (int i = 0; i < sibling->contentSize; i++) {
				node->value[node->contentSize + i] = sibling->value[i];
				node->pointers[node->contentSize + i] = sibling->pointers[i];
			}
			node->contentSize += sibling->contentSize;
			node->pointers[M] = sibling->pointers[M];
			_deleteNodeFunc(sibling);
			_delInternal();
		}
	}

	return true;
}

TEMP_DEF
void TEMP_TREE::_delInternal()
{
	auto re = _stack.back();
	_stack.pop_back();
	TEMP_NODE* node = re->node;
	int position = re->position;
	int begin = position == 0 ? 0 : position - 1;
	for (int i = begin; i < node->contentSize; i++) {
		node->value[i] = node->value[i + 1];
		node->pointers[i + 1] = node->pointers[i + 2];
	}
	_modifyNodeFunc(node);
	node->contentSize--;
	if (_stack.empty()) { // root
		if (_root->contentSize == 0) {
			/*
			int oldId = _root->id;
			memcpy(_root, _getChildFunc(_root->pointers[0]), sizeof(_root));
			_root->id = oldId;
			_modifyNodeFunc(_root);*/
			_root = _getChildFunc(_root->pointers[0]);
		}
	}
	else {
		auto parentRe = _stack.back();
		TEMP_NODE* parentNode = parentRe->node;
		_modifyNodeFunc(parentNode);
		int parentPos = parentRe->position;
		if (node->contentSize < (M + 1) / 2 - 1) {
			if (parentPos != 0) {
				TEMP_NODE* sibling = _getChildFunc(parentNode->pointers[parentPos - 1]);
				_modifyNodeFunc(sibling);
				if (sibling->contentSize > (M + 1) / 2) {
					for (int i = node->contentSize; i >= 1; i--) {
						node->value[i] = node->value[i - 1];
						node->pointers[i + 1] = node->pointers[i];
					}
					node->pointers[1] = node->pointers[0];
					node->value[0] = parentNode->value[parentPos - 1];
					parentNode->value[parentPos - 1] = sibling->value[sibling->contentSize - 1];
					node->pointers[0] = sibling->pointers[sibling->contentSize];
					node->contentSize++;
					sibling->contentSize--;
				}
				else {
					// merge
					sibling->value[sibling->contentSize] = parentNode->value[parentPos - 1];
					sibling->pointers[sibling->contentSize + 1] = node->pointers[0];
					for (int i = 0; i < node->contentSize; i++) {
						sibling->value[sibling->contentSize + i + 1] = node->value[i];
						sibling->pointers[sibling->contentSize + i + 2] = node->pointers[i + 1];
					}
					sibling->contentSize += node->contentSize + 1;
					_deleteNodeFunc(node);
					_delInternal();
				}
			}
			else {
				TEMP_NODE* sibling = _getChildFunc(parentNode->pointers[parentPos + 1]);
				_modifyNodeFunc(sibling);
				if (sibling->contentSize > (M + 1) / 2) {
					node->pointers[node->contentSize + 1] = sibling->pointers[0];
					node->value[node->contentSize] = parentNode->value[parentPos];
					parentNode->value[parentPos] = sibling->value[0];
					sibling->pointers[0] = sibling->pointers[1];
					for (int i = 0; i < sibling->contentSize - 1; i++) {
						sibling->value[i] = sibling->value[i + 1];
						sibling->pointers[i + 1] = sibling->pointers[i + 2];
					}
					sibling->contentSize--;
					node->contentSize++;
				}
				else {
					node->value[node->contentSize] = parentNode->value[parentPos];
					node->pointers[node->contentSize + 1] = sibling->pointers[0];
					for (int i = 0; i < sibling->contentSize; i++) {
						node->value[node->contentSize + i + 1] = sibling->value[i];
						node->pointers[node->contentSize + i + 2] = sibling->pointers[i + 1];
					}
					node->contentSize += sibling->contentSize + 1;
					_deleteNodeFunc(sibling);
					_delInternal();
				}
			}
		}
	}
}

TEMP_DEF
int TEMP_TREE::_findPos(T values[], T value, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		if (values[i] > value) {
			break;
		}
	}
	return i;
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
	std::cout << std::endl;
}
#endif //_DEBUG

#endif // _B_PLUS_TREE_H_
