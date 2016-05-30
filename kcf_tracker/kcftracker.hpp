#ifndef __KCF_TRACKER__
#define __KCF_TRACKER__
#include "tracker.hpp"
class KCFTracker: public Tracker {
 public:
  KCFTracker(bool hog = true, bool fixed_window = true, bool multiscale = true, bool lab = true);
  ~KCFTracker() {}
  virtual void init(const cv::Rect &roi, const cv::Mat &image);
  virtual cv::Rect update(const cv::Mat &image);

  // parameters
  float interp_factor; // linear interpolation factor for adaptation
  float sigma; // gaussian kernel bandwidth
  float lambda; // regularization
  int cell_size; // HOG cell size
  int cell_sizeQ; // cell size^2, to avoid repeated operations
  float padding; // extra area surrounding the target
  float output_sigma_factor; // bandwidth of gaussian target
  int template_size; // template size
  float scale_step; // scale step for multi-scale estimation
  float scale_weight;  // to downweight detection scores of other scales for added stability

 private:
  // important states
  cv::Mat _alphaf;
  cv::Mat _tmpl;
  cv::Mat _prob;
  float _scale;

  int size_patch[3]; //size of the feature
  cv::Mat hann; // hann window
  cv::Size _tmpl_sz;
  
  // feature
  cv::Mat _labCentroids;

  int _gaussian_size;
  bool _hogfeatures;
  bool _labfeatures;


  /*
   * if initHann,
   *  for general initialization: 
   *    compute the template size to do fft by scaling the largest size of the first roi to template_size when using fixed-size window
   *    round template size to be even or even multiple of cell_size if
   *    hog_feature
   *  compute hann window (a cosine shaped window) to make feature on the
   *    boundary equal to zero to satisfy periodicity (continuity) assumption
   *
   *  last_roi (respect true image size) = _scale * tmpl_sz
   *  extracted(new)_roi = last_roi * scale_adjust
   *  resize the extracted_roi to tmpl_sz and compute features on it
   */
  cv::Mat getFeatures(const cv::Mat &img, bool initHann, float scale_adjust = 1.0);

  /*
   * update
   *  find the new alpha for the new detected patch x
   *  interp:
   *    _alpha: the weights for K
   *    _tmpl: the template feature (x) for detection later
   *  FIXME: It's kind of wierd since _alpha does not directly corresponding to the
   *    weights for _tmpl. why not compute the new _alpha directly from the
   *    newly interpolated _tmpl
   */
  void train(const cv::Mat &x, float train_interp_factor);


  /*
   * FIXME: can threshold peak_value to see whether there is a person or not
   * detect x from z and store the peak response in peak_value
   */
  cv::Point2f detect(const cv::Mat &z, const cv::Mat &x, float &peak_value);

  cv::Mat gaussianCorrelation(const cv::Mat &x1, const cv::Mat &x2);

  // the regression target is a gaussian centered at the center. compute this
  // gaussian and then apply fft to it.
  cv::Mat createGaussianPeak(int sizey, int sizex);
  
  // create hann window function to multiply to the feature 
  void createHanningMats();

  // Calculate sub-pixel peak for one dimension
  //float subPixelPeak(float left, float center, float right);
};
#endif
