#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

using namespace std;

#define SPACE_KEY_CODE   32
#define ESCAPE_KEY_CODE  27

enum ErrorType
{
    ER_LOAD_FACE_MODULE = 0,
    ER_OPEN_WEBCAM
};

const char *error_msgs[] = 
{
    "Can't load face module",
    "Can't open webcam"
};

std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
std::string face_shape_path = "shape_predictor_68_face_landmarks.dat";
std::string window_name = "faceOff";

int error_happened(const ErrorType error_type)
{
    cout << error_msgs[error_type] << endl;
    return -1;
}

void draw_faces(const cv::Mat &image, std::vector<cv::Rect> &faces)
{
    using namespace cv;

    Scalar red_color = Scalar(0, 0, 255);
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        ellipse(image, center, 
                Size(faces[i].width / 2, faces[i].height / 2),
                0, 0, 360, red_color, 4);
    }
}

void detect_faces(cv::CascadeClassifier &face_cascade, const cv::Mat &gray_image, std::vector<cv::Rect> &faces)
{
    using namespace cv;

    faces.clear();

    equalizeHist(gray_image, gray_image);
    face_cascade.detectMultiScale(gray_image, faces, 1.21, 3, 0, Size(30, 30));
}

int track_faces(std::vector<cv::Rect> &faces)
{
    // do face tracking here
    // return number of succeeded tracked faces
    
    // temporally, we clear all detected faces here
    // the code should be removed when a face tracking algorithm is implemented.
    faces.clear();

    return (int)faces.size();
}

void draw_landmarks(cv::Mat &image, const dlib::full_object_detection &face_shape)
{
    cv::Scalar blue_color = cv::Scalar(255, 0, 0);
    int total_landmark = face_shape.num_parts();
    for (int i = 0; i < total_landmark; i++)
    {
        dlib::point landmark = face_shape.part(i);
        cv::circle(
            image, cv::Point(landmark.x(), landmark.y()),
            3, blue_color);
    }
}

void detect_landmarks(dlib::shape_predictor &face_shape_predictor, cv::Mat &image, const std::vector<cv::Rect> &faces)
{
    if (faces.empty())
        return;

    dlib::rectangle dlib_face = dlib::rectangle(faces[0].x, faces[0].y, faces[0].x + faces[0].width, faces[0].y + faces[0].height);

    int width = image.cols;
    int height = image.rows;
    int channel = image.channels();

    dlib::array2d<dlib::rgb_pixel> dlib_image(height, width);
    for (int y = 0; y < height; y++)
    {
        uchar* p_data = image.ptr<uchar>(y);
        for (int x = 0; x < width; x++)
        {
            dlib_image[y][x] = dlib::rgb_pixel(p_data[2], p_data[1], p_data[0]);
            p_data += channel;
        } 
    }

    dlib::full_object_detection shape = face_shape_predictor(dlib_image, dlib_face);
    draw_landmarks(image, shape);
}

void morph_faces(cv::Mat &image, const std::vector<cv::Rect> &faces, dlib::shape_predictor &face_shape_predictor)
{
    detect_landmarks(face_shape_predictor, image, faces);
}

int main(int, char**)
{
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_name))
        return error_happened(ER_LOAD_FACE_MODULE);

    dlib::shape_predictor face_shape_predictor;
    dlib::deserialize(face_shape_path.c_str()) >> face_shape_predictor;

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return error_happened(ER_OPEN_WEBCAM);

    cv::namedWindow(window_name, 1);
    std::vector<cv::Rect> faces;
    bool is_in_morphing_mode = false;
    for (;;)
    {
        cv::Mat frame;
        cap >> frame;

        cv::Mat image;
        cv::flip(frame, image, 1);

        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

        if (faces.empty())
        {
            detect_faces(face_cascade, gray_image, faces);
        } 
        else
        {
            if (track_faces(faces) <= 0)
            {
                // when tracking failed, re-detect faces
                detect_faces(face_cascade, gray_image, faces);
            }
        }
     
        if (is_in_morphing_mode)
            morph_faces(image, faces, face_shape_predictor);

        draw_faces(image, faces);

        cv::imshow(window_name, image);
        
        int key_code = cv::waitKey(10);
        bool is_leaving = (key_code == ESCAPE_KEY_CODE);
        if (is_leaving)
            break;

        // enter / leave morphing mode
        if (key_code == SPACE_KEY_CODE)
            is_in_morphing_mode = !is_in_morphing_mode; 
    }

    return 0;
}