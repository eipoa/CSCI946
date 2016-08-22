//============================================================================
// Name        : CSCI935-Ass01-CV.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

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

int main(int argc, char **argv) {
	cout << "Assignment" << endl; // prints Assignment
	InputParser argument(argc, argv);
	string in_file = argument.getCmdOption("-i");
	string out_file = argument.getCmdOption("-o");

	cv::Mat img, gray;
	gray = cv::imread("D:/UOW/2016Spring/CSCI935/assignment1/test1.bmp", IMREAD_UNCHANGED);
	cv::imshow("orig1nal", gray);
	//The two letters C1 and C2 in the conversion constants CV_Bayer C1C2 2BGR and
	//CV_Bayer C1C2 2RGB indicate the particular pattern type. These are components from the second row, second
	//and third columns, respectively. For example, the above pattern has a very popular ¡°BG¡± type.
	cv::cvtColor(gray, img, COLOR_BayerGR2RGB);
	cv::imshow("modified", img);
	//cv::resize(gray, img, Size(gray.cols, gray.rows), 0, 0, INTER_LINEAR);
	//cv::cvtColor(gray, gray, COLOR_BayerGB2RGB, 3);
	cv::imwrite("D:/UOW/2016Spring/CSCI935/assignment1/t1cv.bmp", img);
	cv::waitKey();

	return 0;
}
