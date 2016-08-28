//============================================================================
// Name        : CSCI803-heap-test.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

template<typename T, int size>
void siftdown(T (&heap)[size], int n) {
	// c= left child index
	int c = n * 2 + 1;
	T tmp;
#ifdef NDEBUG
	cout << "n = " << n << "  c = " << c << "  c+1 = " << c + 1 << endl;
#endif
	// the node n has no children
	if (c >= size) {
#ifdef NDEBUG
		cout << "    the node n has no children!" << endl;;
#endif
		return;
	}

	// if node n has right child
	if (c + 1 < size) {
		// compare two children, c= the biggest child' index
		if (heap[c] < heap[c + 1]) {
#ifdef NDEBUG
			cout << "    compare two children to get the biggest one." << endl;
			cout << "      left heap[c] = " << heap[c];
			cout << "      right heap[c+1] = " << heap[c + 1] << endl;
#endif
			c = c + 1;
		}
	} else {
#ifdef NDEBUG
		cout << "    the node n has no right children!" << endl;;
#endif
	}
#ifdef NDEBUG
	cout << "n = " << n << "  c = " << c << endl;
	cout << "    compare the node and the biggest child." << endl;
	cout << "      node heap[n] = " << heap[n] << "  biggest child heap[c] = " << heap[c] << endl;
#endif

	// compare node and biggest child
	if (heap[n] < heap[c]) {
		// less than child, then node move down(imagine a tree, node moves to the lower level)
#ifdef NDEBUG
		cout << "    the node is less than the biggest child." << endl;
		cout << "      swap(heap[n] < heap[c])" << endl;
		cout << "      old array[";
		for (int i = 0; i < size; ++i) {
			if (i + 1 == size) {
				cout << heap[i]<< "]" << endl;
			} else {
				cout << heap[i] << ", ";
			}
		}
#endif
		tmp = heap[n];
		heap[n] = heap[c];
		heap[c] = tmp;
#ifdef NDEBUG
		cout << "      new array[";
		for (int i = 0; i < size; ++i) {
			if (i + 1 == size) {
				cout << heap[i]<< "]" << endl;
			} else {
				cout << heap[i] << ", ";
			}
		}
#endif
		siftdown<int, 8>(heap, c);
	}
}

int main() {
	cout << "Make a Heap Test array[7, 2, 9, 5, 1, 3, 8, 4]" << endl; // prints Assignment
	int size = 8;
	int arr[8] = { 7, 2, 9, 5, 1, 3, 8, 4 };
	for (int i = size / 2; i >= 0; --i) {
		siftdown<int, 8>(arr, i);
	}
#ifdef NDEBUG
	cout << endl << "main() array[";
	for (int i = 0; i < 8; ++i) {
		if (i + 1 == size) {
			cout << arr[i]<< "]" << endl << endl;
		} else {
			cout << arr[i] << ", ";
		}
	}
#endif
	return 0;
}
