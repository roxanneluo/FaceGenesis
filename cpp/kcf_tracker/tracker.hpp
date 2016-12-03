#ifndef __TRACKER__
#define __TRACKER__
#include <opencv2/opencv.hpp>
class Tracker {
 public:
  virtual void init(const cv::Rect& roi, const cv::Mat &image) = 0;
  virtual cv::Rect update(const cv::Mat &image) = 0;
 protected:
  cv::Rect_<float> _roi;
};
#endif
