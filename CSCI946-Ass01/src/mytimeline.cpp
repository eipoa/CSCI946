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

void formatDateTime(string &t) {
	string szShortDays[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	string szShortMonths[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	if (t.length() == 25) {
		// 2007-06-30T16:06:24-04:00
		//cout << t << " -> ";
		t = t.substr(0, 10) + " " + t.substr(11, 8);
		//cout << t << endl;
	} else if (t.length() == 24) {
		// Sun Sep  2 07:27:20 2007
		string sd = t.substr(0, 3);
		string sm = t.substr(4, 3);
		for (int i = 0; i < 7; ++i) {
			if (sd == szShortDays[i]) {
				sd = convert<string, int>(i + 1);
				if (sd.length() == 1) {
					sd = "0" + sd;
				}
			}
		}
		for (int i = 0; i < 12; ++i) {
			if (sm == szShortMonths[i]) {
				sm = convert<string, int>(i + 1);
				if (sm.length() == 1) {
					sm = "0" + sm;
				}
			}
		}
		//cout << t << " -> ";
		if (t.substr(8, 1) == " ")
			t = t.substr(20, 4) + ":" + sm + ":0" + t.substr(9, 1) + " " + t.substr(11, 8);
		else
			t = t.substr(20, 4) + ":" + sm + ":" + t.substr(8, 2) + " " + t.substr(11, 8);
		//cout << t << endl;
	} else if (t.length() == 19) {
		// Sat Jun 14 19:35:12 or 2008:06:22 16:12:58
		//cout << t << " -> ";
		string sd = t.substr(0, 3);
		for (int i = 0; i < 7; ++i) {
			if (sd == szShortDays[i]) {
				t = "0000:00:00 00:00:00";
				break;
			}
		}
		//cout << t << endl;
	} else {
		// cannot recognize
		//cout << t << " -> ";
		t = "0000:00:00 00:00:00";
		//cout << t << endl;
	}
}

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
				/*if (row == "-Date and Time" && imgf.peek() != EOF) {
				 getline(imgf, dt1);
				 formatDateTime(dt1);
				 //break;
				 }*/
				if (row == "-Date and Time (Original)" && imgf.peek() != EOF) {
					getline(imgf, dt2);
					formatDateTime(dt2);
					break;
				}
			}
			imgf.close();
		}
		// chose an appropriate datetime
		dt = dt2;
		ImageInfo img(convert<int, string>(*it), dt);
		result.insert(img);
	}

	string outstr = "-q " + pquery + " -f ";
	for (set<ImageInfo>::iterator it = result.begin(); it != result.end(); ++it) {
		if (it->getDatetime() != "")
			outstr = outstr + convert<string, int>(it->getDocId()) + "*" + it->getDatetime() + ",";
		else
			outstr = outstr + convert<string, int>(it->getDocId()) + ",";
	}
	cout << outstr;
	return 0;
}
