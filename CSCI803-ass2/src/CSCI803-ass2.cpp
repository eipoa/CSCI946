//============================================================================
// Name        : CSCI803-ass2.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Data Structure - Simulation in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <exception>

using namespace std;

// class for Customer
struct Customer {
	friend ostream & operator<<(ostream &out, const Customer &obj) {
		out << setw(12) << left << obj.arrive_time << "  " << setw(12) << left << obj.server_time;
		return out;
	}
	double arrive_time;
	double server_time;
	Customer() {
		arrive_time = 0;
		server_time = 0;
	}
	Customer(double a, double s) :
			arrive_time(a), server_time(s) {
	}
	bool operator <(const Customer &e) const {
		return arrive_time < e.arrive_time;
	}
	bool operator >(const Customer &e) const {
		return arrive_time > e.arrive_time;
	}
};

// class for Heap or queue
// note the element 0 is forbidden to use!!!
// the first element is from 1 !!!
template<class T>
struct Heap {
	friend ostream & operator<<(ostream &out, const Heap & obj) {
		out << "size = " << obj.size << "     count = " << obj.count << endl;
		// please overload << for T object
		for (int i = 1; i <= obj.count; ++i) {
			out << i << "  " << obj[i] << endl;
		}
		out << "size = " << obj.size << "     count = " << obj.count << endl;
		return out;
	}
	// size of the space of heap;
	int size;
	// count of all available element;
	int count;
	// Maximum lenght of queue
	int maxLength;
	// dynamic array for storing object T;
	T *queue;
	// heap type smallest on top(0) all biggest on top(1)
	int type;
	// initial fifty space;
	Heap() {
		type = 0;
		size = 51; // the index is from 1, so 51 is better
		count = 0;
		maxLength = 0;
		queue = new T[size]; // 1..50
	}
	// delete all space
	~Heap() {
		delete[] queue;
	}
	// apply new space
	void extend(int c = 50) {
		T *tmp;
		// allocate new space
		tmp = new T[size + c];
		size = size + c;
		// copy elements, please overload = to do deep copy
		for (int i = 1; i <= count; i++) {
			tmp[i] = queue[i];
		}
		// deallocate old space
		delete[] queue;
		queue = tmp;
	}
	// small root heap
	void siftup0(int i) {
		// it is root
		if (i == 1)
			return;
		// get father
		int p = i / 2;
		if (queue[p] > queue[i]) {
			// if father > child
			T tmp;
			tmp = queue[i];
			queue[i] = queue[p];
			queue[p] = tmp;
			siftup0(p);
		}
	}
	void siftup1(int i) {
		// it is root
		if (i == 1)
			return;
		// get father
		int p = i / 2;
		if (queue[p] < queue[i]) {
			// if father < child
			T tmp;
			tmp = queue[i];
			queue[i] = queue[p];
			queue[p] = tmp;
			siftup0(p);
		}
	}
	// small root heap
	void siftdown0(int i) {
		// left child
		int c = i * 2;
		// node i has no children
		if (c > count) {
			return;
		}
		// if node i has right child
		// chose the smallest node
		if (c + 1 <= count) {
			if (queue[c] > queue[c + 1]) {
				c = c + 1;
			}
		}
		if (queue[i] > queue[c]) {
			// if father < child
			T tmp;
			tmp = queue[i];
			queue[i] = queue[c];
			queue[c] = tmp;
			siftdown0(c);
		}
	}
	void siftdown1(int i) {
		// left child
		int c = i * 2;
		// node i has no children
		if (c > count) {
			return;
		}
		// if node i has right child
		// chose the smallest node
		if (c + 1 <= count) {
			if (queue[c] < queue[c + 1]) {
				c = c + 1;
			}
		}
		if (queue[i] < queue[c]) {
			// if father > child
			T tmp;
			tmp = queue[i];
			queue[i] = queue[c];
			queue[c] = tmp;
			siftdown0(c);
		}
	}
	void push(T e) {
		// the queue is full
		if (count == size - 1) {
			extend();
		}
		// add object at the tail
		count++;
		queue[count] = e;
		if(count>maxLength){
			maxLength = count;
		}
		if (type == 0)
			siftup0(count);
		else
			siftup1(count);
	}
	void pop() {
		//
		//Event tmp=queue[0];
		queue[1] = queue[count];
		//queue[count]=tmp;
		count--;
		if (type == 0)
			siftdown0(0);
		else
			siftdown1(0);
	}
	T & operator[](const int idx) const {		// if const?
		if (idx > count)
			throw 500;
		return queue[idx];
	}
};

// class for service window
struct Window {
	int id;
	double server_end_time;
	bool busy;
	Heap<Customer> customer_queue;
	Heap<Customer> servered_customer_queue;
	// calculate the total service time for new customer's enqueue strategy
	// chose the smallest windows
	Window() {
		id = 0;
		server_end_time = 0;
		busy = false;
	}
	double getWaitTime() {
		double alltime = 0;
		for (int i = 1; i <= customer_queue.count; i++) {
			alltime = alltime + customer_queue[i].server_time;
		}
		return server_end_time + alltime;
	}
	int getMaximunLength(){
		return customer_queue.maxLength;
	}
};

/*struct Server: public Heap {
 double idelTime;

 };*/

int main() {
	cout << "Data Structure - Simulation (ass2)" << endl;
	string filename;
	cout << "Please input filename:";
	getline(cin, filename);
#ifdef NDEBUG
	cout << "Now, processing file [ " << filename << "]" << endl;
#endif
	Heap<Customer> queue;
	int _windowCount;
	double _arriveTime, _serverTime;
	string line;
	fstream fp(filename.c_str(), ios::in);
	if (fp.is_open()) {
		if (!fp.eof() && fp.good()) {
			fp >> _windowCount;
			fp.sync();
		}
#ifdef NDEBUG
		cout << "The total servers is " << _windowCount << "." << endl;
#endif
		while (fp >> _arriveTime >> _serverTime) {
			fp.sync();
			Customer tmp(_arriveTime, _serverTime);
			queue.push(tmp);
		}
		fp.close();
	} else {
		cout << "cannot open file [ " << filename << "]" << endl;
	}

	// initialize simulation
	Window *win = new Window[_windowCount];
	for (int i = 0; i < _windowCount; ++i) {
		win[i].id = i;
	}

	cout << queue;
	return 0;
}
