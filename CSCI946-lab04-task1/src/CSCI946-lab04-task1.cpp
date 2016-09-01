//============================================================================
// Name        : CSCI946-lab04-task1.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "Matrix.h"
using namespace std;

// a simple argument parser
// can only parse -a xxx -b xxx -c xxx -d xxx
class InputParser {
private:
	std::vector<string> tokens;
public:
	InputParser(int &argc, char **argv) {
		for (int i = 1; i < argc; ++i)
			this->tokens.push_back(string(argv[i]));
	}

	const string getCmdOption(const string &option) const {
		vector<string>::const_iterator itr;
		itr = find(this->tokens.begin(), this->tokens.end(), option);
		if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
			return *itr;
		}
		return "";
	}
};

template<class out_type, class in_value>
out_type convert(const in_value & v) {
	stringstream stream;
	stream << v;
	out_type result;
	stream >> result;
	return result;
}

int main(int argc, char ** argv) {
	cout << endl << "              To calculate PageRank of a web graph" << endl; // prints Assignment
	double teleportation_probability = 0;
	string init_Matrix_File = "";
	int Dimension = 7;
	// xMatrix (1, Dimension);
	Matrix<double> xMatrix(1, Dimension);
	// pMatrix (Dimension, Dimension);
	Matrix<double> pMatrix(Dimension, Dimension);
	double data[49] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1 };
	pMatrix.setData(data, 49);
	cout << endl << "   ----------------------- Step 0 aMatrix --------------------" << endl;
	cout << pMatrix << endl;
	double count[Dimension] = { 0 };
	// count the number of 1 for each rows
	for (int r = 0; r < Dimension; ++r) {
		for (int c = 0; c < Dimension; ++c) {
			if (pMatrix(r, c) == 1)
				count[r]++;
		}
		// dead ends
		if (count[r] == 0)
			count[r] = Dimension;
	}
	// calculate the proportion of jumping between them
	for (int r = 0; r < Dimension; ++r) {
		for (int c = 0; c < Dimension; ++c) {
			pMatrix(r, c) = pMatrix(r, c) / count[r];
		}
	}
	cout << "   ----------------------- Step 1 aMatrix --------------------" << endl;
	cout << pMatrix << endl;
	InputParser argument(argc, argv);
	try {
		teleportation_probability = convert<double, string>(argument.getCmdOption("-t"));
		init_Matrix_File = argument.getCmdOption("-v");
		if (init_Matrix_File == "")
			throw 500;
	} catch (...) {
		cout << "Invalid parameters." << endl;
		cout << "(Usage) ./calcu_ pagerank.exe -t 0.07 -v init_vector.txt" << endl;
		return 500;
	}

	cout << endl << "   ----------------------- Step 2 pMatrix --------------------" << endl;
	// convert A Matrix to P Matrix
	// pMatrix = aMatrix * (1-alpha) + alpha/N;
	pMatrix = pMatrix * (1 - teleportation_probability) + (teleportation_probability * 1.0) / Dimension;
	cout << pMatrix << endl;

	fstream fp(init_Matrix_File.c_str(), ios::in);
	if (fp.is_open()) {
		// read initial vector x0 data from file
		for (int c = 0; c < Dimension; ++c) {
			fp >> xMatrix(0, c);
			if (!fp.good())
				break;
		}
		fp.close();
	}

	cout << endl << "   ---------------------- Step 3 xVextor ------------------------------" << endl;
	cout << "   X000: " << xMatrix;
	// get the steady states
	Matrix<double> xMatrixOld;
	xMatrixOld = xMatrix;
	xMatrix = xMatrix * pMatrix;
	int k = 1;
	cout << "   X" << setw(3) << setfill('0') << k << ": " << xMatrix;
	while (xMatrixOld != xMatrix) {
		xMatrixOld = xMatrix;
		xMatrix = xMatrix * pMatrix;
		k++;
		cout << "   X" << setw(3) << setfill('0') << k << ": " << xMatrix;
	}

	return 0;
}
