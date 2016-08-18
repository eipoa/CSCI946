//============================================================================
// Name        : search.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : search in C++, Ansi-style
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
	set<int> getdocIDList() {
		return docIDList;
	}
	;
	void add(int d) {
		if (docIDList.insert(d).second)
			freq++;
	}
};

// implementation of intersection algorithm
set<int> intersection(set<int> l, set<int> r) {
	set<int> result;
	set<int>::iterator itl, itr;
	itl = l.begin();
	itr = r.begin();
	while (itl != l.end() && itr != r.end()) {
		// if find the same docID
		if (*itl == *itr) {
			result.insert(*itl);
			itl++;
			itr++;
		} else {
			// advance iterator at the less side
			if (*itl < *itr)
				itl++;
			else
				itr++;
		}
	}
	return result;
};

void showUsage() {
	cout << "Usage: ./search -c collection.txt -i inverted_index.txt" << endl;
};

int main(int argc, char **argv) {
	//cout << "lab01-task02" << endl; // prints lab01-task01
	// default parameters
	string infile = "collection.txt", outfile = "inverted_index.txt";
	int pindex = 1;
	while (pindex < argc) {
		if (strcmp(argv[pindex], "-c") == 0) {
			pindex++;
			if (pindex < argc)
				infile = argv[pindex++];
			else {
				showUsage();
				return 1;
			}
		} else if (strcmp(argv[pindex], "-i") == 0) {
			pindex++;
			if (pindex < argc)
				outfile = argv[pindex++];
			else {
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

	// parse a query
	bool halt = false;
	string query = "rise AND july";
	multimap<int, set<int> > sortedpostlist;
	// split the query into multimap sorted by frequency of the terms in query
	size_t pos = query.find(" AND ");
	while (pos != string::npos) {
		term = query.substr(0, pos);
		transform(term.begin(), term.end(), term.begin(), ::tolower);
#ifdef NDEBUG
		cout << "key = " << term << endl;
#endif
		if (term != "") {
			// find the keyword within dictionary
			it = DictPostings.find(term);
			if (it != DictPostings.end()) {
				sortedpostlist.insert(
						pair<int, set<int> >(it->second.getFreq(),
								it->second.getdocIDList()));
			} else {
				// the keyword is not in the dictionary, so no result;
				halt = true;
				break;
			}
		}
		query.erase(0, pos + 5);
		pos = query.find(" AND ");
	}
	// the last one term
	if (query != "" && !halt) {
		transform(query.begin(), query.end(), query.begin(), ::tolower);
#ifdef NDEBUG
		cout<<"key = "<<query<<endl;
#endif
		it = DictPostings.find(query);
		if (it != DictPostings.end()) {
			sortedpostlist.insert(
					pair<int, set<int> >(it->second.getFreq(),
							it->second.getdocIDList()));
		} else {
			// the keyword is not in the dictionary, so no result;
			halt = true;
		}
	}

	set<int> result;
	if (halt) {
		// the keyword is not in the dictionary, so no result;
		// do nothing
	} else {
		multimap<int, set<int> >::iterator itquery;
#ifdef NDEBUG
		for (itquery = sortedpostlist.begin(); itquery != sortedpostlist.end();
				++itquery) {
			cout << left << setw(2) << itquery->first << " : ";
			copy(itquery->second.begin(), itquery->second.end(),
					ostream_iterator<int>(cout, " "));
			cout << endl;
		}
#endif
		itquery = sortedpostlist.begin();
		// intermediate result, chose the shortest posting list
		result = itquery->second;
		itquery++;
		while (itquery != sortedpostlist.end()) {
			// intermediate result
			result = intersection(result, itquery->second);
			itquery++;
		}
	}

	// output
	fstream outf(outfile.c_str(), ios::out);
	copy(result.begin(), result.end(), ostream_iterator<int>(outf, " "));
	return 0;
}
