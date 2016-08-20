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

vector<string> getFileNames(string ids) {
	vector<string> result;
	string id;
	size_t pos = ids.find(",");
	while (pos != string::npos) {
		id = ids.substr(0, pos);
		if (id != "") {
			id = "im" + id + ".jpg";
			result.push_back(id);
		}
		ids.erase(0, pos + 1);
		pos = ids.find(",");
	}
	if (ids != "") {
		ids = "im" + ids + ".jpg";
		result.push_back(ids);
	}
	return result;
}
;

string getHtmlHeader(string query) {
	string html = "";
	html = "<!doctype html>";
	html = html + "<html>";
	html = html + "<head>";
	html = html + "	<meta charset='UTF-8'>";
	html = html + "	<meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1'>";
	html = html + "	<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
	html = html + "	<title>Image retrieve result</title>";
	html = html + "	<link rel='stylesheet' type='text/css' href='css/normalize.css' />";
	html = html + "	<link rel='stylesheet' type='text/css' href='css/default.css'>";
	html = html + "	<style>";
	html = html + "	@font-face{font-family:'Calluna';";
	html = html + "		src:url('fonts/callunasansregular-webfont.woff') format('woff');";
	html = html + "	}";
	html = html + "	body {";
	html = html + "		background: url(img/scribble_light.png);";
	html = html + "		font-family: Calluna, Arial, sans-serif;";
	html = html + "		min-height: 1000px;";
	html = html + "	}";
	html = html + "	#columns {";
	html = html + "		column-width: 150px;/*320px;*/";
	html = html + "		column-gap: 15px;";
	html = html + "		width: 90%;";
	html = html + "		max-width: 1100px;";
	html = html + "		margin: 50px auto;";
	html = html + "	}";
	html = html + "	div#columns figure {";
	html = html + "		background: #fefefe;";
	html = html + "		border: 2px solid #fcfcfc;";
	html = html + "		box-shadow: 0 1px 2px rgba(34, 25, 25, 0.4);";
	html = html + "		margin: 0 2px 15px;";
	html = html + "		padding: 15px;";
	html = html + "		padding-bottom: 10px;";
	html = html + "		transition: opacity .4s ease-in-out;";
	html = html + "		display: inline-block;";
	html = html + "		column-break-inside: avoid;";
	html = html + "	}";
	html = html + "	div#columns figure img {";
	html = html + "		width: 100%; height: auto;";
	html = html + "		border-bottom: 1px solid #ccc;";
	html = html + "		padding-bottom: 15px;";
	html = html + "		margin-bottom: 5px;";
	html = html + "	}";
	html = html + "	div#columns figure figcaption {";
	html = html + "		font-size: .9rem;";
	html = html + "		color: #444;";
	html = html + "		line-height: 1.5;";
	html = html + "	}";
	html = html + "	div#columns small { ";
	html = html + "		font-size: 1rem;";
	html = html + "		float: right; ";
	html = html + "		text-transform: uppercase;";
	html = html + "		color: #aaa;";
	html = html + "	} ";
	html = html + "	div#columns small a { ";
	html = html + "		color: #666; ";
	html = html + "		text-decoration: none; ";
	html = html + "		transition: .4s color;";
	html = html + "	}";
	html = html + "	div#columns:hover figure:not(:hover) {";
	html = html + "		/*opacity: 0.4;*/";
	html = html + "	}";
	html = html + "	@media screen and (max-width: 750px) { ";
	html = html + "		#columns { column-gap: 0px; }";
	html = html + "		#columns figure { width: 100%; }";
	html = html + "	}";
	html = html + "	</style>";
	html = html + "</head>";
	html = html + "<body>";
	html = html + "	<div class='htmleaf-container'>";
	html = html + "		<header class='htmleaf-header'>";
	html = html + "			<h1> Query: " + query + "</h1>";
	html = html + "		</header>";
	html = html + "		<div id='columns'>";
	return html;
}
;

string getHtmlBody(vector<string> imgs) {
	string html = "";
	for (vector<string>::iterator it = imgs.begin(); it != imgs.end(); ++it) {
		html = html + "			<figure>";
		html = html + "				<img src='../a1data/thumbnails/" + (*it) + "'>";
		html = html + "				<figcaption>" + (*it) + "</figcaption>";
		html = html + "			</figure>";
	}
	return html;
}

string getHtmlFooter() {
	string html = "";
	html = html + "		</div>";
	html = html + "	</div>";
	html = html + "</body>";
	html = html + "</html>";
	return html;
}
;

int main(int argc, char** argv) {
	InputParser argument(argc, argv);
	string phtmlFile = "./html/" + argument.getCmdOption("-p");
	string pquery = argument.getCmdOption("-q");
	string pIDs = argument.getCmdOption("-f");

	// get all IDs
	vector<string> fileNames = getFileNames(pIDs);
	string html = getHtmlHeader(pquery);
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

