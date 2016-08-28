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

struct Event {
	friend ostream & operator<<(ostream &out, const Event &obj) {
		out << setw(12) << left << obj.arrive_time << "  " << setw(12) << left << obj.server_time;
		return out;
	}
	double arrive_time;
	double server_time;
	Event() {
		arrive_time = 0;
		server_time = 0;
	}
	Event(double a, double s) :
			arrive_time(a), server_time(s) {
	}
	bool operator <(const Event &e) const {
		return arrive_time < e.arrive_time;
	}
	bool operator >(const Event &e) const {
		return arrive_time > e.arrive_time;
	}
};

struct Simulation {
	friend ostream & operator<<(ostream &out, const Simulation & obj) {
		out << "size = " << obj.size << "     count = " << obj.count << endl;
		for (int i = 0; i < obj.count; ++i) {
			out << i << "  " << obj[i] << endl;
		}
		out << "size = " << obj.size << "     count = " << obj.count << endl;
		return out;
	}
	// size of the heap;
	int size;
	// count of all available Event;
	int count;
	// dynamic array for storing Event object;
	Event *queue;
	// initial fifty space;
	Simulation() {
		size = 50;
		count = 0;
		queue = new Event[size];
	}
	;
	// delete all space
	~Simulation() {
		delete[] queue;
	}
	// apply new space
	void extend(int c = 50) {
		Event *tmp;
		// allocate new space
		tmp = new Event[size + c];
		size = size + c;
		// copy elements
		for (int i = 0; i < count; i++) {
			tmp[i] = queue[i];
		}
		// deallocate old space
		delete[] queue;
		queue = tmp;
	}
	// small root heap
	void siftup(int i) {
		// it is root
		if (i == 0)
			return;
		int p = (i - 1) / 2;
#ifdef NDEBUG
		cout << "i = " << i << "  p = " << p << endl;
#endif
		if (queue[p] < queue[i]) {
			return;
		} else {
			// if ftaher > child
#ifdef NDEBUG
			cout << "before " << *this;
#endif
			Event tmp;
			tmp = queue[i];
			queue[i] = queue[p];
			queue[p] = tmp;
#ifdef NDEBUG
			cout << "after " << *this;
#endif
			siftup(p);
		}
	}
	// small root heap
	void siftdown(int i) {
		// left child
		int c = i * 2 + 1;
		// node i has no children
		if (c >= count) {
			return;
		}
		// if node i has right child
		// chose the smallest node
		if (c + 1 < count) {
			if (queue[c + 1] < queue[c]) {
				c = c + 1;
			}
		}
		if (queue[c] > queue[i]) {
			// if ftaher < child
			Event tmp;
			tmp = queue[i];
			queue[i] = queue[c];
			queue[c] = tmp;
			siftdown(c);
		}
	}
	void push(Event e) {
		// the queue is full
		if (count == size) {
			extend();
		}
		queue[count] = e;
#ifdef NDEBUG
		cout << "push : "<< e << count << endl;
#endif
		count++;
		siftup(count - 1);
	}
	void pop() {
		//Event tmp=queue[0];
		queue[0] = queue[count - 1];
		//queue[count]=tmp;
		count--;
		siftdown(0);
	}
	Event & operator[](const int idx) const {
		if (idx > count - 1)
			throw 500;
		return queue[idx];
	}
};

int main() {
	cout << "Data Structure - Simulation (ass2)" << endl;
	string filename;
	cout << "Please input filename:";
	getline(cin, filename);
#ifdef NDEBUG
	cout << "Now, processing file [ " << filename << "]" << endl;
#endif
	Simulation queue;
	int _serverCount;
	double _arriveTime, _serverTime;
	string line;
	fstream fp(filename.c_str(), ios::in);
	if (fp.is_open()) {
		if (!fp.eof() && fp.good()) {
			fp >> _serverCount;
			fp.sync();
		}
#ifdef NDEBUG
		cout << "The total servers is " << _serverCount << "." << endl;
#endif
		while (fp >> _arriveTime >> _serverTime) {
			fp.sync();
			Event tmp(_arriveTime, _serverTime);
			queue.push(tmp);
		}
		fp.close();
	} else {
		cout << "cannot open file [ " << filename << "]" << endl;
	}
	cout << queue;
	return 0;
}
