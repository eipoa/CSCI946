//============================================================================
// Name        : CSCI946-Ass01.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Create a HTML generator program called createwebpage.cpp
//   			1. This program will take the following option:
//				 -p mywebpage.html which represents the output file inHTML format.
//				2. The program will assume that all images are stored in the subdirectory called
//				   thumbnails in the current directory.
//				3. This program will read keywords followed by numbers from standard input as the
//				   image IDs and generate an HTML file to display all retrieved images with the
//				   image ID under each image. You should insert some text at the beginning of the
//				   file to indicate the search terms etc.
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

struct _Image {
	string _docId;
	string _datetime;
	_Image(string id, string d) {
		_docId = id;
		_datetime = d;
	}
	;
};

vector<_Image> getFileNames(string ids) {
	vector<_Image> result;
	string id;
	size_t pos = ids.find(",");
	size_t posStar;
	string docid = "";
	string dt = "";
	while (pos != string::npos) {
		id = ids.substr(0, pos);
		if (id != "") {
			posStar = id.find("*");
			if (posStar != string::npos) {
				docid = "im" + id.substr(0, posStar) + ".jpg";
				dt = id.substr(posStar + 1);
			} else {
				docid = "im" + id + ".jpg";
				dt = "";
			}
			result.push_back(_Image(docid, dt));
		}
		ids.erase(0, pos + 1);
		pos = ids.find(",");
	}
	if (ids != "") {
		id = ids;
		posStar = id.find("*");
		if (posStar != string::npos) {
			docid = "im" + id.substr(0, posStar) + ".jpg";
			dt = id.substr(posStar + 1);
		} else {
			docid = "im" + id + ".jpg";
			dt = "";
		}
		result.push_back(_Image(docid, dt));
	}
	return result;
}
;

string getHtmlHeader(string query) {
	string html = "";
	html = "<!doctype html>\n";
		html = html + "<html>\n";
		html = html + "<head>\n";
		html = html + "	<meta charset='UTF-8'>\n";
		html = html + "	<meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1'>\n";
		html = html + "	<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
		html = html + "	<title>Image retrieve result</title>\n";
		//html = html + "	<link rel='stylesheet' type='text/css' href='./html/css/normalize.css' />\n";
		//html = html + "	<link rel='stylesheet' type='text/css' href='./html/css/default.css'>\n";
		html = html + "	<style>\n";
		html = html + "body, html {\n";
		html = html + "    font-size: 100%;\n";
		html = html + "    padding: 0;\n";
		html = html + "    margin: 0;\n";
		html = html + "	font-family: Calluna, Arial, sans-serif;\n";
		html = html + "}\n";
		html = html + "*, *::after, *::before {\n";
		html = html + "	box-sizing: border-box;\n";
		html = html + "}\n";
		html = html + "body{\n";
		html = html + "	color: #404d5b;\n";
		html = html + "	font-weight: 500;\n";
		html = html + "	font-size: 1.05em;\n";
		html = html + "}\n";
		html = html + ".htmleaf-header {\n";
		html = html + "    padding: 1em 190px 1em;\n";
		html = html + "    letter-spacing: -1px;\n";
		html = html + "    text-align: center;\n";
		html = html + "}\n";
		html = html + "article, aside, details, figcaption, figure, footer, header, hgroup, \n";
		html = html + "main, nav, section, summary {\n";
		html = html + "    display: block;\n";
		html = html + "}\n";
		html = html + ".htmleaf-header {\n";
		html = html + "    letter-spacing: -1px;\n";
		html = html + "    text-align: center;\n";
		html = html + "}\n";
		html = html + ".htmleaf-header h1 {\n";
		html = html + "    font-weight: 600;\n";
		html = html + "    font-size: 2em;\n";
		html = html + "    line-height: 1;\n";
		html = html + "    margin-bottom: 0;\n";
		html = html + "    font-family: 'Segoe UI', 'Lucida Grande', Helvetica, Arial, \n";
		html = html + "	'Microsoft YaHei', FreeSans, Arimo, 'Droid Sans', 'wenquanyi micro hei', \n";
		html = html + "	'Hiragino Sans GB', 'Hiragino Sans GB W3', 'FontAwesome', sans-serif;\n";
		html = html + "}\n";
		html = html + "h1 {\n";
		html = html + "    margin: 0.67em 0;\n";
		html = html + "}\n";
		html = html + "#columns {\n";
		html = html + "    width: 90%;\n";
		html = html + "    max-width: 1100px;\n";
		html = html + "    margin: 50px auto;\n";
		html = html + "}\n";
		html = html + "div#columns figure {\n";
		html = html + "    background: #fefefe;\n";
		html = html + "    border: 2px solid #fcfcfc;\n";
		html = html + "    box-shadow: 0 1px 2px rgba(34, 25, 25, 0.4);\n";
		html = html + "    margin: 0 2px 15px;\n";
		html = html + "    padding: 15px;\n";
		html = html + "    padding-bottom: 10px;\n";
		html = html + "    transition: opacity .4s ease-in-out;\n";
		html = html + "    display: inline-block;\n";
		html = html + "    column-break-inside: avoid;\n";
		html = html + "}\n";
		html = html + "div#columns figure img {\n";
		html = html + "    width: 100%;\n";
		html = html + "    height: auto;\n";
		html = html + "    border-bottom: 1px solid #ccc;\n";
		html = html + "    padding-bottom: 15px;\n";
		html = html + "    margin-bottom: 5px;\n";
		html = html + "}\n";
		html = html + "img {\n";
		html = html + "    border: 0;\n";
		html = html + "}\n";
		html = html + "div#columns figure figcaption {\n";
		html = html + "    font-size: .9rem;\n";
		html = html + "    color: #444;\n";
		html = html + "    line-height: 1.5;\n";
		html = html + "}\n";
		html = html + "		@font-face{font-family:'Calluna';\n";
		html = html + "			src:url('./callunasansregular-webfont.woff') format('woff');\n";
		html = html + "		}\n";
		html = html + "	body {\n";
		//html = html + "		background: url(./html/img/scribble_light.png);\n";
		html = html + "		background-color: #EEE;\n";
		html = html + "		font-family: Calluna, Arial, sans-serif;\n";
		html = html + "		min-height: 1000px;\n";
		html = html + "	}\n";
		html = html + "	#columns {\n";
		html = html + "		column-width: 150px;/*320px;*/\n";
		html = html + "		column-gap: 15px;\n";
		html = html + "		width: 90%;\n";
		html = html + "		max-width: 1100px;\n";
		html = html + "		margin: 50px auto;\n";
		html = html + "	}\n";
		html = html + "	div#columns figure {\n";
		html = html + "		background: #fefefe;\n";
		html = html + "		border: 2px solid #fcfcfc;\n";
		html = html + "		box-shadow: 0 1px 2px rgba(34, 25, 25, 0.4);\n";
		html = html + "		margin: 0 2px 15px;\n";
		html = html + "		padding: 15px;\n";
		html = html + "		padding-bottom: 10px;\n";
		html = html + "		transition: opacity .4s ease-in-out;\n";
		html = html + "		display: inline-block;\n";
		html = html + "		column-break-inside: avoid;\n";
		html = html + "	}\n";
		html = html + "	div#columns figure img {\n";
		html = html + "		width: 100%; height: auto;\n";
		html = html + "		border-bottom: 1px solid #ccc;\n";
		html = html + "		padding-bottom: 15px;\n";
		html = html + "		margin-bottom: 5px;\n";
		html = html + "	}\n";
		html = html + "	div#columns figure figcaption {\n";
		html = html + "		font-size: .9rem;\n";
		html = html + "		color: #444;\n";
		html = html + "		line-height: 1.5;\n";
		html = html + "	}\n";
		html = html + "	div#columns small { \n";
		html = html + "		font-size: 1rem;\n";
		html = html + "		float: right; \n";
		html = html + "		text-transform: uppercase;\n";
		html = html + "		color: #aaa;\n";
		html = html + "	} \n";
		html = html + "	div#columns small a { \n";
		html = html + "		color: #666; \n";
		html = html + "		text-decoration: none; \n";
		html = html + "		transition: .4s color;\n";
		html = html + "	}\n";
		html = html + "	div#columns:hover figure:not(:hover) {\n";
		html = html + "		/*opacity: 0.4;*/\n";
		html = html + "	}\n";
		html = html + "	@media screen and (max-width: 750px) { \n";
		html = html + "		#columns { column-gap: 0px; }\n";
		html = html + "		#columns figure { width: 100%; }\n";
		html = html + "	}\n";
		html = html + "	</style>\n";
		html = html + "</head>\n";
		html = html + "<body>\n";
		html = html + "	<div class='htmleaf-container'>\n";
		html = html + "		<header class='htmleaf-header'>\n";
		html = html + "			<h1> Query: " + query + "</h1>\n";
		html = html + "		</header>\n";
		html = html + "		<div id='columns'>\n";
	return html;
}
;

string getHtmlBody(vector<_Image> imgs) {
	string html = "";
	for (vector<_Image>::iterator it = imgs.begin(); it != imgs.end(); ++it) {
		html = html + "			<figure>\n";
		html = html + "				<img src='./thumbnails/" + it->_docId + "'>\n";
		if (it->_datetime == "0000:00:00 00:00:00") {
			html = html + "				<figcaption>" + it->_docId + "</figcaption>\n";
		} else {
			html = html + "				<figcaption>" + it->_docId + "<br>\n";
			html = html + it->_datetime + "</figcaption>\n";
		}
		html = html + "			</figure>\n";
	}
	return html;
}

string getHtmlFooter() {
	string html = "";
	html = html + "		</div>\n";
	html = html + "	</div>\n";
	html = html + "</body>\n";
	html = html + "</html>\n";
	return html;
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

int main(int argc, char** argv) {
	InputParser argument(argc, argv);
	string phtmlFile = argument.getCmdOption("-p");

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

	// get all IDs
	vector<_Image> fileNames = getFileNames(pIDs);
	string html = getHtmlHeader(pquery + " / result: " + convert<string, int>(fileNames.size()) + "");
	html = html + getHtmlBody(fileNames);
	html = html + getHtmlFooter();

	// generate HTML
	fstream outf(phtmlFile.c_str(), ios::out);
	if (outf.is_open()) {
		outf << html;
		outf.close();
	}

	return 0;
}

