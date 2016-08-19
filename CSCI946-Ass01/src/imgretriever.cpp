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
	for (int i = 1; i < argc; ++i)
		query.insert(string(argv[i]));
	return query;
}
;

/**
 * @Descript
 * 		the class is for storing posting list
 */
class PostingList {
private:
	// filename, it's not useful
	string _fn;
	// annotation
	string _keyWord;
	// the frequency of the annotation in whole collections
	int _freq;
	// a sorted and unique list for storing docID
	set<int> _docIDList;
public:
	// before we open an annotation file, we must new a PostingList.
	PostingList() {
		_fn = "";
		_keyWord = "";
		_freq = 0;
		_docIDList.clear();
	}
	;
	// the parameter is the filename which is also a annotation word
	// so the function reads file line by line to generate a posting list
	void loadFile(string fn) {
		_fn = fn;
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
	// to avoid duplicate when inserting the object into a set
	// the set will hold all posting list (unique)
	bool operator==(const PostingList& x) const {
		return this->_keyWord = x._keyWord;
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

int main(int argc, char** argv) {
	// parse the arguments
	set<string> query;
	query = InputParser(argc, argv);

	// the whole posting list which contains all keywords
	map<string, PostingList> postingListCollection;
	// the special posting list which only includes a few keywords which are in the query
	// and all list is sorted by frequency in ascending
	set<PostingList, less<comparePostingList>()> targetPostingList;

	// retrieve all annotation files from current directory
	DIR *dir = NULL;
	struct dirent *drnt = NULL;
	dir = opendir("");
	if (dir) {
		cout << "output:" << endl;
		while (drnt = readdir(dir)) {
			printf("%-20s\t", drnt->d_name);
		}
		closedir(dir);
	} else {
		cout << "Can not open directory" << endl;
	}
	return 0;
}
