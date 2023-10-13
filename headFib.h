#include <iostream>
#include <cmath>

using namespace std;

typedef struct Node {
	int key; // ключ - число
	char *value; // значение - слово
	Node* parent, * child, * left, * right; // указатели на узлы 
	int degree; // степень узла
	bool mark; // флаг удаления узлов
} Node;

typedef struct fibheap {
	int size; // количество узлов в куче
	Node* min; // указатель на минимальный узел
} fibheap;

// основные функции

Node* fib_insert(fibheap* heap, int key, char* value);
Node* fib_min(fibheap* heap);
fibheap* fib_union(fibheap* heapOne, fibheap* heapTwo);
fibheap* fib_delete_min(fibheap* heap);
fibheap* fib_decrease_key(fibheap* heap, Node* node, int newkey);
void fib_delete(fibheap* heap, Node* x);

// дополнительные

fibheap* create_heap();
void FibHeapAddNodeToRootList(Node* node, Node* heap_min);
void deleteMemoryHeap(fibheap* heap);
Node* FibHeapLinkLists(Node* heapOne, Node* heapTwo);
void FibHeapRemoveNodeFromRootList(Node* node, Node* heap);
void fibConsolidate(fibheap* heap);
void fibCut(fibheap* heap, Node* x, Node* y);
void FibCascadingCut(fibheap* heap, Node* y);
Node* heap_lookup(Node* node, int key);