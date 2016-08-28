//============================================================================
// Name        : CSCI803-ass2.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Data Structure - Simulation in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// type cast
template<class out_type, class in_value>
out_type convert(const in_value & v) {
	stringstream stream;
	stream << v;
	out_type result;
	stream >> result;
	return result;
}

int main() {
	cout << "Data Structure - Simulation (ass2)" << endl;
	string filename;
	cout << "Please input filename:";
	getline(cin, filename);
#ifdef NDEBUG
	cout << "Now, processing file [ " << filename << "]" << endl;
#endif
	int _serverCount;
	double _arriveTime, _serverTime;
	string line;
	fstream fp(filename.c_str(), ios::in);
	if (fp.is_open()) {
		if (!fp.eof())
			getline(fp, line);
		_serverCount = convert<int, string>(line);
#ifdef NDEBUG
		cout << "The total servers is " << _serverCount << "." << endl;
#endif
		while (!fp.eof()) {
			fp >> _arriveTime >> _serverTime;
			fp.ignore(1024, '\n');
			cout << _arriveTime << " " << _serverTime << endl;
			break;
		}
		fp.close();
	}
	return 0;
}
