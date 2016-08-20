//============================================================================
// Name        : CSCI946-Ass01.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Create a filter program called mytimeline.cpp
//   			1. The program will read the EXIF files from the sub-directory
//                 called exif in the current directory.
//				2. This program will read numbers from the standard input as the
//                 image IDs, sort them in the descending order of the date and
//                 time when the photographs were originally taken, and print all
//                 image IDs in the sorted order on the standard output. Note that
//                 there are a few properties in EXIF metadata associated with
//                 date and time and use the appropriate one.
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

class ImageInfo {
private:
	int _docID;
	string _datetime;
public:
	ImageInfo(int id, string d) :
			_docID(id), _datetime(d) {
	}
	;

	bool operator==(const ImageInfo &r) {
		return this->_docID == r.getDocId();
	}
	;

	const string& getDatetime() const {
		return _datetime;
	}
	;

	int getDocId() const {
		return _docID;
	}
	;
};

// compare class for ImageInfo
// order by datetime, docid desc;
struct compImageInfo {
	bool operator()(const ImageInfo &l, const ImageInfo &r) {
		if (l.getDatetime() == r.getDatetime())
			return l.getDocId() > r.getDocId();
		return l.getDatetime() >= r.getDatetime();
	}
};

vector<string> getFileNames(string ids) {
	vector<string> result;
	string id;
	size_t pos = ids.find(",");
	while (pos != string::npos) {
		id = ids.substr(0, pos);
		if (id != "") {
			//id = "exif" + id + ".txt";
			result.push_back(id);
		}
		ids.erase(0, pos + 1);
		pos = ids.find(",");
	}
	if (ids != "") {
		//ids = "exif" + ids + ".txt";
		result.push_back(ids);
	}
	return result;
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

string getAppropriateDT(string &dt1, string &dt2){
	string dt;

	return dt;
}
;
int main(int argc, char** argv) {
	// get query & IDs
	string pquery;
	string pIDs;
	string row;
	if (!cin.eof()) {
		getline(cin, row);
	};
	size_t posq = row.find("-q ");
	size_t posf = row.find("-f ");
	if (posq != string::npos && posf != string::npos) {
		if (posq < posf) {
			pquery = row.substr(posq + 3, posf - posq - 4);
			pIDs = row.substr(posf + 3);
		} else {
			pIDs = row.substr(posf + 3, posq - posf - 4);
			pquery = row.substr(posq + 3);
		}
	}

	// read files to get info of images
	// -Date and Time (Original)
	// note that some images have no infomation
	// so give them a uniform datetime "0000:00:00 00:00:00"
	vector<string> fileNames = getFileNames(pIDs);
	set<ImageInfo, compImageInfo> result;
	for (vector<string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
		string fn = "./exif/exif" + (*it) + ".txt";
		fstream imgf(fn.c_str(), ios::in);
		string row, dt = "", dt1 = "", dt2 = "";
		if (imgf.is_open()) {
			while (!imgf.eof()) {
				getline(imgf, row);
				// if find the key row
				if (row == "-Date and Time" && imgf.peek() != EOF) {
					getline(imgf, dt1);
				}
				if (row == "-Date and Time (Original)" && imgf.peek() != EOF) {
					getline(imgf, dt2);
					//break;
				}
			}
			imgf.close();
		}
		// chose an appropriate datetime
		if (dt == "")
			dt = "0000:00:00 00:00:00";
		ImageInfo img(convert<int, string>(*it), dt);
		result.insert(img);
	}

	string outstr = "-q " + pquery + " -f ";
	for (set<ImageInfo>::iterator it = result.begin(); it != result.end(); ++it) {
		outstr = outstr + convert<string, int>(it->getDocId()) + "*" + it->getDatetime() + ",";
	}
	cout << outstr;
	return 0;
}
