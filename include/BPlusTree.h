#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

#include <functional>
#include "Exception.h"
#define TEMP_DEF template <typename T, typename P, int M>
#define TEMP_NODE INode<T, P, M>
#define TEMP_TREE BPlusTree<T, P, M>

TEMP_DEF
struct INode {
	bool isLeaf;
	T value[M - 1];
	P pointers[M]; // If isLeaf = true, then pointers[0] points to the next leaf.
};

TEMP_DEF
class BPlusTree {
public:
	BPlusTree(INode& root, std::function<INode&(P)> getChildFunc, 
		std::function<void(INode)> saveNodeFunc,
		std::function<void(INode)> deleteNodeFunc)
		: _root(root), _getChildFunc(getChildFunc), _saveNodeFunc(saveNodeFunc), _deleteNodeFunc(deleteNodeFunc) {}
	INode& find(T value) const;
	void insert(T value);
	void del(INode& value);
	INode& next(INode& leafNode) const;
private:
	INode& _root;
	std::function<INode&(P)> _getChildFunc;
	std::function<void(INode)> _saveNodeFunc;
	std::function<void(INode)> _deleteNodeFunc;
};

/* Implementation */
TEMP_DEF
inline TEMP_NODE& TEMP_TREE::next(TEMP_NODE& leafNode) const
{
	if (!leafNode.isLeaf) {
		throw SQLException("Attemp to iterate a non-leaf node in B+ tree!");
	}
	return _getChildFunc(leafNode.pointers[0]);
}

TEMP_DEF
TEMP_NODE& TEMP_TREE::find(T value) const 
{

}

TEMP_DEF
void TEMP_TREE::insert(T value)
{

}

TEMP_DEF
void TEMP_TREE::del(TEMP_NODE& value)
{
	
}

#endif // _B_PLUS_TREE_H_
