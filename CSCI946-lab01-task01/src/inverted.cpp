//============================================================================
// Name        : inverted.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : inverted in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <set>
#include <map>
#include <iomanip>
#include <iterator>
#include <algorithm>

using namespace std;

class PostingList {
	friend ostream & operator<<(ostream &out, const PostingList &obj) {
		out << left << setw(2) << " ( " << obj.getFreq() << " ) ";
		copy(obj.docIDList.begin(), obj.docIDList.end(),
				ostream_iterator<int>(out, " "));
		return out;
	}
private:
	int freq;
	set<int> docIDList;
public:
	PostingList() {
		freq = 0;
		docIDList.clear();
	}
	;
	void initPostingList(int d) {
		docIDList.clear();
		docIDList.insert(d);
		freq = 1;
	}
	;
	int getFreq() const {
		return freq;
	}
	;

	void add(int d) {
		// insert docId into posting list
		// and increase freq
		if (docIDList.insert(d).second)
			freq++;
	}
};

void showUsage() {
	cout << "Usage: ./inverted -c collection.txt -o inverted_index.txt" << endl;
}
;

int main(int argc, char **argv) {
	// cout << "lab01-task01" << endl; // prints lab01-task01
	// default parameters
	//string infile = "collection.txt", outfile = "inverted_index.txt";
	string infile, outfile;
	int pindex = 1;
	while (pindex < argc) {
		if (strcmp(argv[pindex], "-c") == 0) {
			pindex++;
			if (pindex < argc) {
				infile = argv[pindex++];
			} else {
				showUsage();
				return 1;
			}
		} else if (strcmp(argv[pindex], "-o") == 0) {
			pindex++;
			if (pindex < argc) {
				outfile = argv[pindex++];
			} else {
				showUsage();
				return 1;
			}
		} else {
			showUsage();
			return 1;
		}
	}

	map<string, PostingList> DictPostings;
	map<string, PostingList>::iterator it;
	pair<map<string, PostingList>::iterator, bool> ret;
	string line, term;
	int docID;
	PostingList p;

	// generate dictionary & posting list
	fstream inf(infile.c_str(), ios::in);
	if (inf.is_open()) {
		while (!inf.eof()) {
			inf >> docID;
			getline(inf, line);
			// convert to lower case
			transform(line.begin(), line.end(), line.begin(), ::tolower);
			// split word
			size_t pos = line.find(" ");
			while (pos != string::npos) {
				term = line.substr(0, pos);
				if (term != "") {
					// insert into posting list
					p.initPostingList(docID);
					ret = DictPostings.insert(
							pair<string, PostingList>(term, p));
					if (!ret.second) {
						// duplicate term, add to posting list
						// the container is set (unique, sorted)
						ret.first->second.add(docID);
					}
				}
				line.erase(0, pos + 1);
				pos = line.find(" ");
			}
			if (line != "") {
				term = line;
				p.initPostingList(docID);
				ret = DictPostings.insert(pair<string, PostingList>(term, p));
				if (!ret.second) {
					// duplicate term, add to posting list
					// the container is set (unique, sorted)
					ret.first->second.add(docID);
				};
			}
		}
	} else {
		cout << "cannot open file collection.txt" << endl;
		return 1;
	}

	// output
	fstream outf(outfile.c_str(), ios::out);
	for (it = DictPostings.begin(); it != DictPostings.end(); ++it)
		outf << left << setw(10) << it->first << it->second << '\n';

	return 0;
}
