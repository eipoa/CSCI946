//============================================================================
// Name        : CSCI946-lab02.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5082497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <functional>

using namespace std;

void showUsage() {
	cout
			<< "Usage: ./searchv -t reuters-tf.dat -d reuters-df.dat best car insurance "
			<< endl;
}
;

class TermVector {
private:
	double tf;
	//double df;
	double idf;
public:
	TermVector() {
		tf = 0;
		//df = 0;
		idf = 0;
	}
	TermVector(double _tf, double _idf) {
		tf = _tf;
		//df = 0;
		idf = _idf;
	}
	/*
	 double getDf() const {
	 return df;
	 }

	 void setDf(double df) {
	 this->df = df;
	 }
	 */
	double getIdf() const {
		return idf;
	}

	void setIdf(double idf) {
		this->idf = idf;
	}

	double getTf() const {
		return tf;
	}

	void setTf(double tf) {
		this->tf = tf;
	}
};

int main(int argc, char **argv) {
	//cout << "lab02" << endl; // prints lab02
	string tfile, dfile;

	int logbase = 10; // change base for the logarithm
	int termCount = 0; // size of dictionary
	int fileCount = 3; //

	vector<string> tmpQuery;
	int N = 3;
	int allCount;
	map<string, double> idf;
	map<string, TermVector> docVector, query;
	map<int, map<string, TermVector> > docs;
	int pindex = 1;
	while (pindex < argc) {
		if (strcmp(argv[pindex], "-t") == 0) {
			pindex++;
			if (pindex < argc)
				tfile = argv[pindex++];
			else {
				showUsage();
				return 1;
			}
		} else if (strcmp(argv[pindex], "-d") == 0) {
			pindex++;
			if (pindex < argc)
				dfile = argv[pindex++];
			else {
				showUsage();
				return 1;
			}
		} else {
			tmpQuery.push_back(argv[pindex++]);
		}
	}

	// initial df idf, get the number of terms
	fstream df(dfile.c_str(), ios::in);
	if (df.is_open()) {
		string term;
		// the last line
		int keypos;
		int termcount;
		while (!df.eof()) {
			df >> term;
			// if the last line
			if (df.peek() != EOF) {
				df >> termcount;
				idf.insert(pair<string, int>(term, termcount));
				termCount++; //how many terms in the dictionary
				keypos = df.tellg();
			} else {
				// get the last line
				df.clear();
				df.seekg(keypos, df.beg);
				df >> allCount;
			}
		}
	}
	cout << "*****************idf*******************" << endl;
	for (map<string, double>::iterator it = idf.begin(); it != idf.end();
			++it) {
		it->second = log10(allCount / it->second) / log10(logbase);
		cout << left << setw(12) << it->first << " " << setw(9) << it->second
				<< endl;
	};

	// intial query
	// insert the same term with idf, query's idf = idf
	for (int i = 0; i < tmpQuery.size(); i++) {
		string term;
		term = tmpQuery[i];
		if (idf.find(term) != idf.end())
			query.insert(
					pair<string, TermVector>(term,
							TermVector(1, idf.find(term)->second)));
	}
	// insert the term in idf not in query with 0
	for (map<string, double>::iterator it = idf.begin(); it != idf.end();
			++it) {
		if (query.find(it->first) == query.end())
			query.insert(pair<string, TermVector>(it->first, TermVector()));
	}
	cout << endl << "****************query******************" << endl;
	double qyeryN2 = 0;	// q1*q1+q2*q2+q3*q3....
	for (map<string, TermVector>::iterator it = query.begin();
			it != query.end(); ++it) {
		qyeryN2 = qyeryN2 + pow(it->second.getIdf(), 2);
		cout << left << setw(12) << it->first << " " << setw(9)
				<< it->second.getTf() << " " << setw(9) << it->second.getIdf()
				<< endl;
	}

	// initial docVector (posting list) and length
	// how to get the length of the collection??
	fstream tf(tfile.c_str(), ios::in);
	vector<double> length(3, 0);
	map<string, vector<double> > postings;
	if (tf.is_open()) {
		// get the number of documents
		string term;
		double tf1, tf2, tf3;
		vector<double> rowtf;
		while (!tf.eof()) {
			tf >> term >> tf1 >> tf2 >> tf3;
			rowtf.clear();
			rowtf.push_back(tf1);
			rowtf.push_back(tf2);
			rowtf.push_back(tf3);
			length[0] = length[0] + pow(tf1, 2);
			length[1] = length[1] + pow(tf2, 2);
			length[2] = length[2] + pow(tf3, 2);
			postings.insert(pair<string, vector<double> >(term, rowtf));
		}
	}
	// tf*idf and rotate? posting list
	map<int, vector<double> > rotatedPosting; // <docID, tf.idf>
	for (int i = 1; i < 4; ++i)
		rotatedPosting.insert(
				pair<int, vector<double> >(i, vector<double>(4, 0)));
	cout << endl << "*****************tf*idf****************" << endl;
	int i = 0;
	for (map<string, vector<double>>::iterator it = postings.begin();
			it != postings.end(); ++it) {
		it->second[0] = idf.find(it->first)->second * it->second[0]
				/ sqrt(length[0]);
		it->second[1] = idf.find(it->first)->second * it->second[1]
				/ sqrt(length[1]);
		it->second[2] = idf.find(it->first)->second * it->second[2]
				/ sqrt(length[2]);

		//rotate
		for (int fileIdx = 0; fileIdx < 3; ++fileIdx) {
			rotatedPosting.find(fileIdx + 1)->second[i] = it->second[fileIdx];
		}
		i++;

		cout << left << setw(12) << it->first << " " << setw(9) << it->second[0]
				<< " " << setw(9) << it->second[1] << " " << setw(9)
				<< it->second[2] << endl;
	}

	// dicplay rotatePosting list
	for (map<int, vector<double>>::iterator it = rotatedPosting.begin();
			it != rotatedPosting.end(); ++it) {
		it->second[4] = pow(it->second[0], 2) + pow(it->second[1], 2)
				+ pow(it->second[2], 2) + pow(it->second[3], 2);
		it->second[4] = sqrt(it->second[4] * qyeryN2);
		cout << left << setw(12) << it->first << " " << setw(9) << it->second[0]
				<< " " << setw(9) << it->second[1] << " " << setw(9)
				<< it->second[2] << setw(9) << it->second[3] << setw(9)
				<< it->second[4] << endl;
	}

	/*  base=10
	 *****************idf*******************
	 auto         2.0792
	 best         1.50476
	 car          1.64753
	 insurance    1.62253

	 ****************query******************
	 auto         0        0
	 best         1        1.50476
	 car          1        1.64753
	 insurance    1        1.62253

	 *****************td*idf****************
	 auto         0.2041   1.46484  0
	 best         0.68932  0        0.619336
	 car          1.45553  0.140693 0.957313
	 insurance    0        1.14311  1.13921

	 base=2
	 *****************idf*******************
	 auto         6.90695
	 best         4.9987
	 car          5.47296
	 insurance    5.38994

	 ****************query******************
	 auto         0        0
	 best         1        4.9987
	 car          1        5.47296
	 insurance    1        5.38994

	 *****************td*idf****************
	 auto         0.678007 4.86611  0
	 best         2.28987  0        2.05739
	 car          4.83518  0.467374 3.18012
	 insurance    0        3.79734  3.78436
	 ***********docid tf.idf=weight*********
	 1            0.678007  2.28987   4.83518  0
	 2            4.86611   0         0.467374 3.79734
	 3            0         2.05739   3.18012  3.78436
	 */

	// calculate score
	vector<double> score(3, 0);
	int j = 0;
	for (map<string, TermVector>::iterator itq = query.begin();
			itq != query.end(); ++itq) {
		int i = 0;
		for (map<int, vector<double>>::iterator itp = rotatedPosting.begin();
				itp != rotatedPosting.end(); ++itp) {
			score[i] = score[i] + itq->second.getIdf() * itp->second[j];
			i++;
		}
		j++;
	}

	cout << endl << "*****************score*****************" << endl;
	map<int, vector<double>>::iterator itp = rotatedPosting.begin();
	for (int i = 0; i < 3; i++) {
		score[i] = score[i] / itp->second[4];
		itp++;
		cout << left << setw(12) << i + 1 << setw(9) << score[i] << endl;
	}
	multimap<double, int, greater<double> > sortedScore;
	for (int i = 0; i < 3; i++) {
		sortedScore.insert(pair<double, int>(score[i],i+1));
	}
	for(multimap<double, int, greater<double> >::iterator it=sortedScore.begin(); it!=sortedScore.end();it++){
		cout<< it->second <<"    "<<it->first<<endl;
	}
	return 0;
}
