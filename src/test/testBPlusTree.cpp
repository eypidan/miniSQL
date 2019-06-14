#include "BPlusTree.h"
#include <cstdlib>
#define SIZE 15

INode<float, 5> nodes[SIZE];
float elements[SIZE];
int top = 2;

void shuffle() {
	for (int i = 0; i < SIZE - 1; i++) {
		for (int j = i + 1; j < SIZE; j++) {
			if (rand() % 2) {
				float t = elements[i];
				elements[i] = elements[j];
				elements[j] = t;
			}
		}
	}
}

int main() {
	for (int i = 0; i < SIZE; i++) {
		nodes[i].id = i;
		nodes[i].contentSize = 0;
		nodes[i].isLeaf = true;
		elements[i] = i;
	}
	
	BPlusTree<float, 5> tree(&nodes[1], [](int id) {
		return &nodes[id];
	}, [](auto node) {
		nodes[node->id] = *node;
	}, [](auto node) {
		
	}, []() {
		return &nodes[top++];
	});
	// test insert
	shuffle();
	for (int i = 0; i < SIZE; i++) {
		tree.insert(elements[i], -1);
	}
	// test find
	shuffle();
	for (int i = 0; i < SIZE; i++) {
		auto re = tree.find(elements[i]);
		if (re->node->value[re->position] != elements[i]) {
			std::cout << "error!!!: " << i << std::endl;
		}
	}
	// test find min/max
	auto min = tree.findExtreme(false);
	if (min->node->value[min->position] != 0) {
		std::cout << "error!!!: min = " << min->node->value[min->position] << std::endl;
	}
	auto max = tree.findExtreme(true);
	if (max->node->value[max->position] != SIZE - 1) {
		std::cout << "error!!!: max = " << max->node->value[max->position] << std::endl;
	}
	// test iteration
	auto current = min->node;
	do {
		std::cout << current->value[0] << " ";
		current = tree.next(current);
	} while (current != max->node);
	std::cout << std::endl;
	tree.printTree();
	return 0;
}

