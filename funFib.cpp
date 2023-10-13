#include "headFib.h"

fibheap* create_heap() {
	fibheap* root = new fibheap;

	root->min = NULL;
	root->size = 0;

	return root;
}

void FibHeapAddNodeToRootList(Node* node, Node* heap_min) {
	if (heap_min == NULL) {
		return;
	}
	
	if (heap_min->left == heap_min) {
		heap_min->left = node;
		heap_min->right = node;
		node->left = heap_min;
		node->right = heap_min;
	}
	else {
		Node* lnode = heap_min->left;
		heap_min->left = node;
		node->right = heap_min;
		node->left = lnode;
		lnode->right = node;
	}
}

Node* fib_insert(fibheap* heap, int key, char* value) {
	Node* node = new Node;
	node->key = key;
	node->value = value;
	node->degree = 0;
	node->mark = false;
	node->parent = NULL;
	node->child = NULL;
	node->left = node;
	node->right = node;

	FibHeapAddNodeToRootList(node, heap->min);
	if (heap->min == NULL || node->key < heap->min->key) {
		heap->min = node;
	}
	heap->size = heap->size + 1;

	return node;
}

Node* fib_min(fibheap* heap) {
	return heap->min;
}

void deleteMemoryHeap(fibheap* heap) {
	if (heap == NULL) {
		return;
	}

	if (heap->min != NULL) {
		Node* current = heap->min;
		Node* first = current;

		while (current != first) {
			Node* next = current->right;
			delete(current);
			current = next;
		}
	}

	delete(heap);
}

Node* FibHeapLinkLists(Node* heapOne, Node* heapTwo) {
	if (heapOne == NULL || heapTwo == NULL) {
		return NULL;
	}

	Node* leftOne = heapOne->left;
	Node* leftTwo = heapTwo->left;
	leftOne->right = heapTwo;
	heapTwo->left = leftOne;
	heapOne->left = leftTwo;
	leftTwo->right = heapOne;

	return heapOne;
}

fibheap* fib_union(fibheap* heapOne, fibheap* heapTwo) {
	fibheap* heap = new fibheap;
	heap->min = heapOne->min;
	FibHeapLinkLists(heapOne->min, heapTwo->min);

	if (heapOne->min == NULL || (heapTwo->min != NULL && heapTwo->min->key < heap->min->key)) {
		heap->min = heapTwo->min;
	}

	heap->size = heapOne->size + heapTwo->size;

	deleteMemoryHeap(heapOne);
	deleteMemoryHeap(heapTwo);

	return heap;
}

void FibHeapRemoveNodeFromRootList(Node* node, Node* heap) {
	if (node == NULL || heap == NULL) {
		return;
	}

	if (node == node->right) {
		heap = NULL;
	}
	else {
		node->left->right = node->right;
		node->right->left = node->left;

		if (node == heap) {
			heap = node->right;
		}
	}
	node->left = node->right = NULL;

}

void fibConsolidate(fibheap* heap) {
	int maxDegree = ((log(heap->size)) / (log((1 + sqrt(5)) / 2)));

	Node** arr = (struct Node**)calloc(maxDegree + 1, sizeof(struct Node*));
	Node* start = heap->min;
	Node* x = start;

	do {
		int d = x->degree;
		while (arr[d] != NULL) {
			Node* y = arr[d];

			if (x->key > y->key) {
				Node* tmp = x;
				x = y;
				y = tmp;
			}

			if (y == start) {
				start = start->right;
			}

			if (y == x->right) {
				x->right = x->right->right;
				x->right->right->left = x;
			}

			else {
				y->left->right = y->right;
				y->right->left = y->left;
			}

			y->left = y;
			y->right = y;

			if (x->right == x) {
				start = y;
			}

			y->parent = x;

			if (x->child == NULL) {
				x->child = y;
			}

			else {
				y->left = x->child;
				y->right = x->child->right;
				x->child->right = y;
				y->right->left = y;
			}

			x->degree++;
			arr[d] = NULL;
			d++;
		}

		arr[d] = x;
		x = x->right;

	} while (x != start);

	heap->min = NULL;

	for (int j = 0; j <= maxDegree; j++) {
		if (arr[j] != NULL) {
			arr[j]->left = arr[j];
			arr[j]->right = arr[j];
			if (heap->min == NULL) {
				heap->min = arr[j];
			}

			else {
				arr[j]->left = heap->min;
				arr[j]->right = heap->min->right;

				heap->min->right = arr[j];

				arr[j]->right->left = arr[j];

				if (arr[j]->key < heap->min->key)
				{
					heap->min = arr[j];
				}
			}
		}
	}
	
	delete(arr);
}

fibheap* fib_delete_min(fibheap* heap) {
	Node* oldMinNode = heap->min;

	if (oldMinNode != NULL)
	{
		if (oldMinNode->child != NULL)
		{
			Node* child = NULL;
			child = oldMinNode->child;
			do
			{
				child->parent = NULL;
				child = child->right;

			} while (child != oldMinNode->child);

			Node* minRight = heap->min->right;
			Node* childLeft = child->left;

			heap->min->right = child;

			child->left = heap->min;
			minRight->left = childLeft;
			childLeft->right = minRight;
		}
		oldMinNode->left->right = oldMinNode->right;
		oldMinNode->right->left = oldMinNode->left;

		if (oldMinNode == oldMinNode->right)
		{
			heap->min = NULL;
		}

		else
		{
			heap->min = oldMinNode->right;
			fibConsolidate(heap);
		}
		heap->size = heap->size - 1;
	}
	return heap;
}

void fibCut(fibheap* heap, Node* x, Node* y) {
	FibHeapRemoveNodeFromRootList(x, y);
	y->degree = y->degree - 1;
	FibHeapAddNodeToRootList(x, heap->min);
	x->parent = NULL;
	x->mark = false;
}

void FibCascadingCut(fibheap* heap, Node* y) {
	Node* z = y->parent;
	
	if (z == NULL) {
		return;
	}

	if (y->mark == false) {
		y->mark = true;
	}
	else {
		fibCut(heap, y, z);
		FibCascadingCut(heap, z);
	}
}

fibheap* fib_decrease_key(fibheap* heap, Node* node, int newkey) {
	if (newkey > node->key) {
		return NULL;
	}

	node->key = newkey;
	Node* y = node->parent;
	
	if (y != NULL && node->key < y->key) {
		fibCut(heap, node, y);
		FibCascadingCut(heap, y);
	}

	if (node->key < heap->min->key) {
		heap->min = node;
	}

	return heap;
}

void fib_delete(fibheap* heap, Node* x) {
	fib_decrease_key(heap, x, -INFINITY);
	fib_delete_min(heap);
}

Node* heap_lookup(Node* node, int key) {
	if (node == NULL) {
		return NULL;
	}

	Node* currentNode = node;
	Node* foundNode = NULL;

	do {
		if (currentNode->key == key) {
			foundNode = currentNode;
			break;
		}

		Node* child = heap_lookup(currentNode->child, key);
		if (child != NULL) {
			foundNode = child;
			break;
		}

		currentNode = currentNode->right;
	} while (currentNode != node);

	return foundNode;
}