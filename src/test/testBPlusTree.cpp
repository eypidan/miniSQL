#include "BPlusTree.h"
#include <cstdlib>
#include <ctime>
#define SIZE 100
#define NodeSize 100

INode<float, NodeSize> nodes[SIZE * 2];
float elements[SIZE];
int top = 2;

void shuffle() {
	for (int i = SIZE; i > 1;i--) {
		int cur = SIZE - i + (rand() % i);
		float tmp = elements[SIZE - i];
		elements[SIZE - i] = elements[cur];
		elements[cur] = tmp;
	}
}

int main() {
	srand(time(NULL));
	
	for (int i = 0; i < SIZE; i++) {
		nodes[i].id = i;
		nodes[i].contentSize = 0;
		nodes[i].isInternal = false;
		elements[i] = (float)i;
	}
	
	BPlusTree<float, NodeSize> tree(&nodes[1], [](int id) {
		return &nodes[id];
	}, [](auto node) {
		//nodes[node->id] = *node;
	}, [](auto node) {
		
	}, []() {
		return &nodes[top++];
	}, [](auto id) {

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
			std::cout << "error A!!!: " << i << std::endl;
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
	tree.printTree();
	// test iteration
	auto current = min->node;
	do {
		std::cout << current->value[0] << " ";
		current = tree.next(current);
	} while (current != max->node && current->id > 0);
	std::cout << std::endl;
	// test delete
	shuffle();
	int i, j;
	for (i = 0; i < SIZE / 2; i++) {
		tree.del(elements[i]);
	}
	tree.printTree();
	for (j = i + 1; j < SIZE; j++) {
		auto re = tree.find(elements[j]);
		if (re->node->value[re->position] != elements[j]) {
			std::cout << "error B!!!: " << j << std::endl;
		}
	}
	for (; i < SIZE - 1; i++) {
		tree.del(elements[i]);
	}
	tree.printTree();
	return 0;
}

