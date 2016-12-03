#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "morph_face.hpp"

using namespace std;

void parseLandmarks(const string &filename, vector<cv::Point>& landmarks) {
  std::ifstream fin(filename.c_str());
  double x, y;
  while(!fin.eof()) {
    fin >> x >> y;
    landmarks.push_back(cv::Point(x, y));
  }
}

int main(int argc, char ** argv) {
  assert(argc > 4);
  string src_landmarks_filename = argv[1];
  string dst_landmarks_filename = argv[2];
  string src_image_filename = argv[3];
  string dst_image_filename = argv[4];

  cv::Mat src_image = cv::imread(src_image_filename.c_str());
  vector<cv::Point> src_landmarks;
  vector<cv::Point> dst_landmarks; 
  parseLandmarks(src_landmarks_filename, src_landmarks);
  parseLandmarks(dst_landmarks_filename, dst_landmarks);
  cv::Mat dst_image = control_faces(src_image, dst_landmarks,
                                    src_image, src_landmarks, false);

  assert(cv::imwrite(dst_image_filename.c_str(), dst_image));

  return 0;
}
