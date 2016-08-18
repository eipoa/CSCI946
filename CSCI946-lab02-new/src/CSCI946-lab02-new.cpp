//============================================================================
// Name        : CSCI946-lab02-new.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5082497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <iterator>

using namespace std;
class Collection {
	friend ostream& operator<<(ostream& out, const Collection &obj) {
		out << endl;
		out << "-----------------------------------------------------------------------------------" << endl;
		out << setw(12) << left << "Terms" << setw(12) << left << "df" << setw(12) << left << "idf=log(N/df) based(" << obj._logBase << ") N="
				<< obj._N << endl;
		out << "-----------------------------------------------------------------------------------" << endl;
		for (map<string, vector<double> >::const_iterator it = obj._termPair.begin(); it != obj._termPair.end(); ++it) {
			out << setw(12) << left << it->first << setw(12) << left << it->second[0] << setw(12) << left << it->second[1] << endl;
		}
		return out;
	}
private:
	map<string, vector<double> > _termPair;  //vector<double>: (df, idf)
	long _N;	// total terms of whole collections
	int _logBase;
public:
	Collection() {
		_N = 0;
		_logBase = 10;
		_termPair.clear();
	}
	pair<map<string, vector<double> >::iterator, bool> insertTermPair(string term, vector<double> other) {
		return _termPair.insert(pair<string, vector<double> >(term, other));
	}
	;
	void setN(long n) {
		_N = n;
	}
	;
	void setLogBase(int l) {
		_logBase = l;
	}
	;
	map<string, vector<double> > & getPair() {
		return _termPair;
	}
	;
	void Calculateidf() {
		for (map<string, vector<double> >::iterator it = _termPair.begin(); it != _termPair.end(); ++it) {
			it->second[1] = log10(_N / it->second[1]) / log10(_logBase);
		}
	}
	;
};
class Query {
	friend ostream& operator<<(ostream& out, const Query &obj) {
		out << "-----------------------------------------------------------------------------------" << endl;
		out << setw(12) << left << "query_Terms" << setw(12) << left << "tf(q)" << setw(25) << left << "weight(t,q)=qtf * idf" << setw(12) << left
				<< "length" << endl;
		out << "-----------------------------------------------------------------------------------" << endl;
		for (map<string, vector<double> >::const_iterator it = obj._termPair.begin(); it != obj._termPair.end(); ++it) {
			if (it != obj._termPair.begin())
				out << setw(12) << left << it->first << setw(12) << left << it->second[0] << setw(25) << left << it->second[1] << endl;
			else
				out << setw(12) << left << it->first << setw(12) << left << it->second[0] << setw(25) << left << it->second[1] << setw(12) << left
						<< obj._length << endl;
		}
		return out;
	}
private:
	map<string, vector<double> > _termPair;  //vector<double>: (tfq, idf, weight)
	double _length;
public:
	Query() {
		_length = 0;
		_termPair.clear();
	}
	;
	pair<map<string, vector<double> >::iterator, bool> insertTermPair(string term, vector<double> other) {
		return _termPair.insert(pair<string, vector<double>>(term, other));
	}
	;
	map<string, vector<double> > & getPair() {
		return _termPair;
	}
	;
	double getLength() {
		return _length;
	}
	;
	void CalculateWeightLength() {
		for (map<string, vector<double> >::iterator it = _termPair.begin(); it != _termPair.end(); ++it) {
			if (it->second[0] == 0) {
				it->second[1] = 0;
			} else {
				_length = _length + pow(it->second[1], 2);
			}
		}
		_length = sqrt(_length);
	}
	;
};

class Document {
	friend ostream& operator<<(ostream& out, const Document &obj) {
		out << setw(12) << left << obj._docID;
		// output word & weight
		for (map<string, vector<double> >::const_iterator it = obj._termPair.begin(); it != obj._termPair.end(); ++it) {
			if (it == obj._termPair.begin())
				out << setw(12) << left << it->first << setw(12) << left << it->second[0] << setw(25) << left << it->second[1] << setw(12) << left
						<< obj._length << setw(12) << left << obj._Score << endl;
			else
				out << setw(12) << left << "" << setw(12) << left << it->first << setw(12) << left << it->second[0] << setw(25) << left
						<< it->second[1] << endl;
		}
		out << "-----------------------------------------------------------------------------------" << endl;
		return out;
	}
private:
	int _docID;
	double _Score;
	map<string, vector<double> > _termPair;  //vector<double>: (tfd, idf, weight)
	double _length;
public:
	Document(int id) {
		_docID = id;
		_Score = 0;
		_length = 0;
		_termPair.clear();
	}
	;
	pair<map<string, vector<double> >::iterator, bool> insertTermPair(string term, vector<double> other) {
		return _termPair.insert(pair<string, vector<double>>(term, other));
	}
	;
	int getDocID() const {
		return _docID;
	}
	;
	void setDocID(int v) {
		_docID = v;
	}
	;
	double getScore() const {
		return _Score;
	}
	;
	map<string, vector<double> > & getPair() {
		return _termPair;
	}
	;
	void CalculateWeightLength(Collection &idf) {
		double idft;
		for (map<string, vector<double> >::iterator it = _termPair.begin(); it != _termPair.end(); ++it) {
			idft = idf.getPair().find(it->first)->second[1];
			it->second.push_back(it->second[0] * idft);
			_length = _length + pow(it->second[1], 2);
		}
		_length = sqrt(_length);
	}
	;
	void CalculateScore(Query &query) {
		for (map<string, vector<double> >::iterator it = _termPair.begin(); it != _termPair.end(); ++it) {
			_Score = _Score + (query.getPair().find(it->first)->second[1] * it->second[1]);
		}
		_Score = _Score / _length / query.getLength();
	}
	;
};
struct compDoc {
	bool operator()(const Document& x, const Document& y) const {
		return x.getScore() > y.getScore();
	}
};
// a simple argument parser
// can only parse -a xxx -b xxx -c xxx -d xxx
// can not parse -a xxx yyy zzz mmm
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

vector<string> split(string str, string delimiter = ",") {
	vector<string> result;
	string a = str;
	size_t pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	if (str.length() != 0)
		result.push_back(str);

#ifdef INFO
	cout << endl;
	cout << "-----------------------------------------------------------------------------------" << endl;
	cout << "split string... [" << a << "]" << endl;
	cout << "-----------------------------------------------------------------------------------" << endl;
	//copy(result.begin(), result.end(), ostream_iterator<string>(cout, " + "));
	int i = 0;
	for (vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
		cout << "result[" << i << "] = " << (*it) << endl;
		i++;
	}
	cout << endl;
#endif

	return result;
}

template<class out_type, class in_value>
out_type convert(const in_value & v) {
	stringstream stream;
	stream << v;
	out_type result;
	stream >> result;
	return result;
}

int main(int agrc, char ** argv) {
	//cout << "lab02-new" << endl; // prints lab02-new
	// parse argument
	// searchv -t reuters-tf.dat -d reuters-df.dat -q best,car,insurance
	InputParser argument(agrc, argv);
	string tf_file = argument.getCmdOption("-t");
	string df_file = argument.getCmdOption("-d");
	string out_file = argument.getCmdOption("-o");
	string queryStr = argument.getCmdOption("-q");
	int K = 2;
	vector<string> tmpVectorString;
	vector<double> tmpVectorDouble;
	vector<double> tmpValue;

	// log file
	fstream outf(out_file.c_str(), ios::out);

	// calculate idf according to file [df_file]
	Collection idf;
	int N;
	// first step: read file, get df & N
	fstream df(df_file.c_str(), ios::in);
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
				tmpValue.clear();
				tmpValue.push_back(termcount);
				tmpValue.push_back(termcount);
				idf.insertTermPair(term, tmpValue);
				keypos = df.tellg();
			} else {
				// get the last line
				df.clear();
				df.seekg(keypos, df.beg);
				df >> N;
				idf.setN(N);
			}
		}
	}
	// second step: calculate idf = log(N/df)
	int logBase = 10;
#ifdef BASE2
	logBase = 2;
#endif
	idf.setLogBase(logBase);
	idf.Calculateidf();
	outf << idf << endl;

	// @generate query
	Query query;
	// first step: copy idfList to queryList
	query.getPair().insert(idf.getPair().begin(), idf.getPair().end());
	// second step: if term in idfList is not in queryStr, then set weight value to 0, otherwise 1
	// split queryString
	tmpVectorString = split(queryStr);
	set<string> qyerySet(tmpVectorString.begin(), tmpVectorString.end());
	for (map<string, vector<double> >::iterator it = query.getPair().begin(); it != query.getPair().end(); ++it) {
		if (qyerySet.find(it->first) == qyerySet.end()) {
			it->second[0] = 0;
		} else {
			it->second[0] = 1;
		}
	}
	// calculate query length
	query.CalculateWeightLength();
	outf << query;

	//@generate documents dictionary
	map<int, Document> document;
	map<int, Document>::iterator it_doc;
	fstream tf(tf_file.c_str(), ios::in);
	if (tf.is_open()) {
		string line;
		while (!tf.eof()) {
			getline(tf, line);
			tmpVectorString = split(line, " ");
			// get the first element which is a word
			// car 27 4 24
			vector<string>::iterator it = tmpVectorString.begin();
			string rowTerm = (*it);
			it++;
			int i = 0;
			for (; it != tmpVectorString.end(); ++it) {
				// find the file ID in documents
				i++;
				it_doc = document.find(i);
				if (it_doc == document.end()) {
					//car 27 4 24
					//0   1  2 3
					//tf*idf
					tmpVectorDouble.clear();
					tmpVectorDouble.push_back(convert<double, string>(tmpVectorString[i]));
					Document doc(i);
					doc.insertTermPair(tmpVectorString[0], tmpVectorDouble);
					// new file ID
					document.insert(pair<int, Document>(i, doc));

				} else {
					// new word dict, tf*idf in existing document
					tmpVectorDouble.clear();
					tmpVectorDouble.push_back(convert<double, string>(tmpVectorString[i]));
					it_doc->second.insertTermPair(tmpVectorString[0], tmpVectorDouble);
				}
			}
		}
	}

	// calculate doc length
	set<Document, compDoc> Score;
	outf << endl;
	outf << "-----------------------------------------------------------------------------------" << endl;
	outf << setw(12) << left << "doc_ID" << setw(12) << left << "terms" << setw(12) << left << "tf(d)" << setw(25) << left << "weight(t,d)=dtf * idf"
			<< setw(12) << left << "length" << setw(12) << left << "score" << endl;
	outf << "-----------------------------------------------------------------------------------" << endl;
	for (it_doc = document.begin(); it_doc != document.end(); ++it_doc) {
		it_doc->second.setDocID(it_doc->first);
		it_doc->second.CalculateWeightLength(idf);
		it_doc->second.CalculateScore(query);
		outf << it_doc->second;
		Score.insert(it_doc->second);
	}

	outf << endl;
	outf << "The top " << K << " files:" << endl;
	outf << "-----------------------------------------------------------------------------------" << endl;
	outf << setw(12) << left << "doc_ID" << setw(12) << left << "terms" << setw(12) << left << "tf(d)" << setw(25) << left << "weight(t,d)=dtf * idf"
			<< setw(12) << left << "length" << setw(12) << left << "score" << endl;
	outf << "-----------------------------------------------------------------------------------" << endl;
	for (set<Document>::iterator it = Score.begin(); it != Score.end(); ++it) {
		outf << (*it);
		K--;
		if (K == 0)
			break;
	}

	return 0;

	/*
g++ -o csci946-lba02 ./src/CSCI946-lab02-new.cpp
g++ -o csci946-lba02 ./src/CSCI946-lab02-new.cpp -DBASE2
./csci946-lba02.exe -t reuters-tf.dat -d reuters-df.dat -q best,car,insurance -o output.log

	 */
}

