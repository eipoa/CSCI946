//============================================================================
// Name        : CSCI946-Ass01.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : The goal of this assignment is to design and implement an image search
//               engine based on image metadata, in particular image annotations.
//
//               1. This program will take the following command-line arguments:
//                    term1 [term2 ...]
//                  which represents an unknown number of search keywords.
//               2. The program will read the annotation files from the subdirectory called
//                  annotation in the current directory.
//               3. The program will print the search keywords and all image IDs that are
//                  labeled with all keywords on the standard output.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <dirent.h>

using namespace std;

/**
 * @Descript
 * 		to parse the arguments, and save them into a set object.
 * @Param
 * 		argc: the number of command line arguments
 * 		argv: the string of all argument
 * @Return
 *      query: it is a sorted and unique argument list
 */
set<string> InputParser(int &argc, char **argv) {
	set<string> query;
	string term;
	for (int i = 1; i < argc; ++i) {
		term = string(argv[i]);
		transform(term.begin(), term.end(), term.begin(), ::tolower);
		query.insert(term);
	}
	return query;
}
;

template<class out_type, class in_value>
out_type convert(const in_value & v) {
	stringstream stream;
	stream << v;
	out_type result;
	stream >> result;
	return result;
}
;

/**
 * @Descript
 * 		the class is for storing posting list
 */
class PostingList {
	friend ostream &operator<<(ostream &out, const PostingList &obj) {
		out << "KeyWord= " << obj.getKeyword() << "  frequency= " << obj.getFreq() << endl;
		//copy(obj._docIDList.begin(), obj._docIDList.end(), ostream_iterator<int>(out, ", "));
		return out;
	}
private:
	// annotation
	string _keyWord;
	// the frequency of the annotation in whole collections
	int _freq;
	// a sorted and unique list for storing docID
	set<int> _docIDList;
public:
	// before we open an annotation file, we must new a PostingList.
	PostingList(string v) {
		_keyWord = v;
		_freq = 0;
		_docIDList.clear();
	}
	;
	// the parameter is the filename which is also a annotation word
	// so the function reads file line by line to generate a posting list
	void loadFile(string fn) {
		string id;
		fstream fp(fn.c_str(), ios::in);
		if (fp.is_open()) {
			while (!fp.eof()) {
				getline(fp, id);
				if (id != "")
					this->addDocument(convert<int, string>(id));
			}
			fp.close();
		}
	}
	;
	// insert into docID into <_docIDList>, if it is inserted successfuly
	// the _freq++
	void addDocument(int docID) {
		pair<set<int>::iterator, bool> ret;
		ret = this->_docIDList.insert(docID);
		if (ret.second)
			_freq++;
	}
	;
	//
	int getFreq() const {
		return this->_freq;
	}
	;
	string getKeyword() const {
		return this->_keyWord;
	}
	;
	set<int> getDocList() const {
		return this->_docIDList;
	}
	;
	// to avoid duplicate when inserting the object into a set
	// the set will hold all posting list (unique)
	bool operator==(const PostingList& x) const {
		return this->_keyWord == x.getKeyword();
	}
};

/**
 * @Descript
 * 		it is a compare class for set object.
 * 		the set will hold all posting list order by frequency
 */
struct comparePostingList {
	bool operator()(const PostingList& x, const PostingList& y) const {
		return x.getFreq() < y.getFreq();
	}
}
;
// get file lists
vector<string> listFiles(string dir) {
	string dirName;
	vector<string> result;
	try {
		DIR *pDIR;
		struct dirent *entry;
		if (pDIR = opendir(dir.c_str())) {
			while (entry = readdir(pDIR)) {
				dirName = entry->d_name;
				if (dirName != "." && dirName != "..") {
					result.push_back(dirName);
				}
			}
			closedir(pDIR);
		}
	} catch (...) {
		result.clear();
	}
	return result;
}
;

string extractFileName(string fn) {
	size_t pos = fn.find(".");
	if (fn.find(".") != string::npos)
		fn.erase(pos);
	return fn;
}

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
}
;
int main(int argc, char** argv) {
	// current directory
	//string currentDir = "D:/UOW/2016Spring/CSCI946/labs/assignment01/a1data/annotation";
	string currentDir = "annotation";
	// parse the arguments
	set<string> query;
	query = InputParser(argc, argv);

	// the whole posting list which contains all keywords
	// if the program only receives query before running, we can just read the files which
	// are relevant to the query!!
	// map<string, PostingList> postingListCollection;

	// the special posting list which only includes a few keywords which are in the query
	// and all list is sorted by frequency in ascending
	set<PostingList, comparePostingList> targetPostingList;

	// retrieve all annotation files from current directory
	vector<string> files = listFiles(currentDir);
	int count= 0;
	for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
		string term = extractFileName(*it);
		transform(term.begin(), term.end(), term.begin(), ::tolower);
		set<string>::iterator ret;
		ret = query.find(term);
		if (ret != query.end()) {
			// new PostingList object with parameter term
			PostingList pl(term);
			// add all doc IDs
			pl.loadFile(currentDir + "/" + (*it));
			// insert into the set targetPostingList
			targetPostingList.insert(pl);
			// all terms have been used
			count++;
			if (count==query.size()) break;
		}
	};

	// to call intersection function
	set<int> result;
	set<PostingList>::iterator itquery;
	itquery = targetPostingList.begin();
	// intermediate result, chose the shortest posting list
	result = itquery->getDocList();
	itquery++;
	while (itquery != targetPostingList.end()) {
		// intermediate result
		result = intersection(result, itquery->getDocList());
		itquery++;
	}

	// generate output
	// -q xxx,xxx,xxx -f id,id,id
	string outstr = "-q ";
	//copy(query.begin(), query.end(), ostream_iterator<string>(cout, ", "));
	//copy(result.begin(), result.end(), ostream_iterator<int>(cout, ", "));
	for (set<string>::iterator it = query.begin(); it != query.end(); ++it) {
		outstr = outstr + (*it) + ",";
	}
	outstr= outstr.substr(0, outstr.length()-1);
	outstr = outstr + " -f ";
	for (set<int>::iterator it = result.begin(); it != result.end(); ++it) {
		outstr = outstr + convert<string, int>(*it) + ",";
	}
	outstr= outstr.substr(0, outstr.length()-1);
	cout<< outstr;

	return 0;
}
