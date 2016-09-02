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
#include <cfloat>

using namespace std;

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
		//out << "size = " << obj.size << "     count = " << obj.count;
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
		queue = NULL;
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
	void siftupMin(int i) {
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
			siftupMin(p);
		}
	}
	void siftupMax(int i) {
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
			siftupMax(p);
		}
	}
	// small root heap
	void siftdownMin(int i) {
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
			siftdownMin(c);
		}
	}
	void siftdownMax(int i) {
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
			siftdownMax(c);
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
		if (count > maxLength) {
			maxLength = count;
		}
		if (type == 0)
			siftupMin(count);
		else
			siftupMax(count);
	}
	void pop() {
		queue[1] = queue[count];
		count--;
		if (type == 0)
			siftdownMin(1);
		else
			siftdownMax(1);
	}
	T & front() {
		return queue[1];
	}
	T & operator[](const int idx) const {		// if const?
		if (idx > count)
			throw 500;
		return queue[idx];
	}
	bool isEmpty() {
		return count == 0;
	}
	int getCount() const {
		return count;
	}
};

// class for Customer
struct Customer {
	friend ostream & operator<<(ostream &out, const Customer &obj) {
		out << setw(12) << left << obj.arrival_time << "  " << setw(12) << left << obj.served_time << "  " << setw(12)
				<< left << obj.winID;
		return out;
	}
	// the arrival time of the customer
	double arrival_time;
	// the served time of the customer
	double served_time;
	// the waiting time of the customer
	double wait_time;
	// wait windID
	int winID;

	Customer() {
		arrival_time = 0;
		served_time = 0;
		wait_time = 0;
		winID = 0;
	}
	Customer(double a, double s) :
			arrival_time(a), served_time(s) {
		wait_time = 0;
		winID = 0;
	}
	bool operator <(const Customer &e) const {
		return arrival_time < e.arrival_time;
	}
	bool operator >(const Customer &e) const {
		return arrival_time > e.arrival_time;
	}
	double getWaitTime() const {
		return wait_time;
	}
	// to calculate the waiting time
	// it equals latest end_time minus arrival_time
	void setWaitTime(double waitTime) {
		wait_time = waitTime;
	}

	double getServedTime() const {
		return served_time;
	}
};

// class for service window
struct Window {
	friend ostream & operator<<(ostream &out, const Window &obj) {
		if (obj.busy)
			out << "busy = true   ";
		else
			out << "busy = false  ";
		out << "currentTime = " << fixed << setprecision(3) << setw(12) << left << setfill(' ') << obj.currentTime;
		out << "serve_end_time = " << fixed << setprecision(3) << setw(12) << left << setfill(' ')
				<< obj.serve_end_time;
		out << "idle_time = " << fixed << setprecision(3) << setw(12) << left << setfill(' ') << obj.idle_time << endl;
		//out << "+++++ served_customer_queue +++++" << endl;
		//out << obj.served_customer_queue << endl;
		return out;
	}
	int id;
	double serve_end_time;
	double idle_time;
	bool busy;
	// each window has a current time!!!
	double currentTime;

	Heap<Customer> customer_queue;
	Heap<Customer> served_customer_queue;

	// calculate the total service time for new customer's enqueue strategy
	// chose the smallest windows
	Window() {
		id = 0;
		serve_end_time = 0;	//DBL_MAX;
		idle_time = 0;
		busy = false;
		currentTime = 0;
	}
	double getWaitTime() {
		double alltime = 0;
		for (int i = 1; i <= customer_queue.count; i++) {
			alltime = alltime + customer_queue[i].served_time;
		}
		return serve_end_time + alltime;
	}
	int getMaximunLength() {
		return customer_queue.maxLength;
	}

	double getAllRearServedTime() {
		double tmp = serve_end_time;
		int customerCount = customer_queue.getCount();
		for (int i = 1; i < customerCount; ++i) {
			tmp = tmp + customer_queue[i].getServedTime();
		}
		return tmp;
	}
};

struct Simulation {
	// 0: one queue; 1: multiqueue
	int simulationType;
	// how many windows for the simulation
	int numWins;
	Window *Wins;
	Heap<Customer> *Customers;
	// data for the simulation
	Heap<Customer> data;
	// close time, for example 6:00pm
	// double CloseTime = 0;
	// the current time
	double currentTime;
	int maxQueueLengh;

	Simulation() {
		simulationType = 0;
		numWins = 0;
		Wins = NULL;
		Customers = NULL;
		currentTime = 0;
		maxQueueLengh = 0;
	}

	// initial windows & queue list
	// the position 0 is forbidden to use
	// the first window & queue is in position 1
	void init() {
		Wins = new Window[numWins];
		if (simulationType == 0)
			Customers = new Heap<Customer> [2];
		else
			Customers = new Heap<Customer> [numWins];
	}
	void finish() {
		delete[] Wins;
		Wins = NULL;
		delete[] Customers;
		Customers = NULL;
	}
	// load customers data from a file
	void LoadData(string fn) {
		double arrival_Time = 0, served_Time = 0;
		string line;
		fstream fp(fn.c_str(), ios::in);
		if (fp.is_open()) {
			// read the first line to get number of total windows
			if (!fp.eof() && fp.good()) {
				fp >> numWins;
				fp.sync();
				// the position 0 is forbidden to use
				numWins++;
			}
			// read customers data line by line, and push them into customer object
			// each line is: 190.9  20.5
			while (fp >> arrival_Time >> served_Time) {
				fp.sync();
				Customer tmp(arrival_Time, served_Time);
				data.push(tmp);
			}
			fp.close();
		} else {
			cout << "cannot open file [ " << fn << "]" << endl;
		}
	}

	double getAllRearServedTime(int id) {
		double tmp = Wins[id].serve_end_time;
		int customerCount = Customers[id].getCount();
		for (int i = 1; i < customerCount; ++i) {
			tmp = tmp + Customers[id][i].served_time;
		}
		return tmp;
	}

	// return id of the window
	// which is not busy or waiting time is smallest (choosing a best window)
	int getBestWin() {
		cout << "getBestWin(): ";
		double minServedTime0 = DBL_MAX;
		double minServedTime1 = DBL_MAX;
		int bestWinId0 = 0, bestWinId1 = 0;
		double endtimeQueue;	// include customers in queue
		for (int i = 1; i < numWins; ++i) {
			if (simulationType == 0) {
				if (!Wins[i].busy) {
					// find a free window and first finish
					if (Wins[i].serve_end_time < minServedTime0) {
						minServedTime0 = Wins[i].serve_end_time;
						bestWinId0 = i;
					}
				}
			} else {
				if (!Wins[i].busy) {
					// find a free window and first finish
					if (Wins[i].serve_end_time < minServedTime0) {
						minServedTime0 = Wins[i].serve_end_time;
						bestWinId0 = i;
					}
				} else {
					endtimeQueue = getAllRearServedTime(i);
					if (endtimeQueue < minServedTime1) {
						minServedTime1 = endtimeQueue;
						bestWinId1 = i;
					}
				}
			}
		}

		if (simulationType == 0) {
			return bestWinId0;
		} else {
			if (bestWinId0 != 0)
				return bestWinId0;
			else
				return bestWinId1;
		}
	}

	void processEvents(int winID, Customer curentData) {
		// ****
		if (simulationType == 0 and winID==0) {
			curentData.winID = winID;
			Customers[1].push(curentData);
			if (maxQueueLengh < Customers[1].count)
				maxQueueLengh = Customers[1].count;
			return;
		}

		if (Wins[winID].busy) {
			// wait for service
			if (simulationType == 0) {
				curentData.winID = winID;
				Customers[1].push(curentData);
				if (maxQueueLengh < Customers[1].count)
					maxQueueLengh = Customers[1].count;
			} else {
				curentData.winID = winID;
				Customers[winID].push(curentData);
			}
		} else {
			// record served customers
			curentData.winID = winID;
			Wins[winID].served_customer_queue.push(curentData);
			// calculate idle time
			if (Wins[winID].serve_end_time != DBL_MAX)
				if(curentData.arrival_time > Wins[winID].serve_end_time)
					Wins[winID].idle_time = Wins[winID].idle_time + curentData.arrival_time - Wins[winID].serve_end_time;
			else
				Wins[winID].idle_time = curentData.arrival_time; // the first customer for this window

			// the window starts service
			Wins[winID].busy = true;
			if(curentData.arrival_time < Wins[winID].serve_end_time)
				Wins[winID].serve_end_time = Wins[winID].serve_end_time + curentData.served_time;
			else
				Wins[winID].serve_end_time = curentData.arrival_time + curentData.served_time;
			cout << Wins[winID] << endl;

		}
	}

	void syncWindows(double &timeIs) {
		cout << "-----------------------------------------------------------------syncWindows(" << timeIs << "):"
				<< endl;
		for (int i = 1; i < numWins; ++i) {
			//Wins[i].currentTime = Wins[i].serve_end_time;
			if (Wins[i].serve_end_time < timeIs) {
				// finish service
				Wins[i].busy = false;
			}
			cout << "window[" << setw(2) << right << setfill('0') << i << "]: ";
			cout << Wins[i];
		}
		//cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
		dealCustomerInQueue(timeIs);
	}

	void dealCustomerInQueue(double &timeIs) {
		Customer currentCust;
		if (simulationType == 0) {
			while (Customers[1].count > 0) {
				currentCust = Customers[1].front();
				// some customers in queue are finished already.
				if (currentCust.arrival_time < timeIs) {
					int winId = getBestWin();
					if (winId == 0) {
						cout << "++no windows are free" << endl;
						break;
					} else {
						cout << "**windows[" << winId << "] is the best." << endl;
					}
					processEvents(winId, currentCust);
					Customers[1].pop();
				} else {
					// 理论上不可能走到这里
					cout << "ERROR" << endl;
					break;
				}
			}
		} else {
			for (int i = 1; i < numWins; ++i) {
				cout << "window[" << setw(2) << right << i << "]: ";			//setfill('0')
				cout << Customers[i] << endl;			//<< setfill('')
				if (Wins[i].busy)
					continue;
				while (Customers[i].count > 0) {
					cout << "<><><><><><>" << endl;
					currentCust = Customers[i].front();
					// some customers in queue are finished already.
					if (currentCust.arrival_time < timeIs) {
						processEvents(i, currentCust);
						Customers[i].pop();
					} else {
						// 理论上不可能走到这里
						break;
					}
				}
			}
		}
	}

	void run() {
		while (!data.isEmpty()) {
			double timeIs = data.front().arrival_time;
			// sync all windows && clear queue
			syncWindows(timeIs);

			// deal with new customer from data
			// which windows is the best
			int winId = getBestWin();
			cout << "##windows[" << winId << "] is the best for new customer." << endl;
			processEvents(winId, data.front());

			// delete current customer data from queue
			data.pop();
		}
		cout << Customers[1] << endl;
		cout << Wins[1].served_customer_queue << endl;
	}
};

int main() {
	cout << "Data Structure - Simulation (ass2)" << endl;
	string filename;
	//cout << "Please input filename:";
	//getline(cin, filename);
	filename = "test.txt";
	Simulation sim;
	sim.LoadData(filename);
	//cout << sim.data << endl;
	//sim.simulationType = 1;
	sim.init();
	sim.run();
	sim.finish();

	return 0;
}
