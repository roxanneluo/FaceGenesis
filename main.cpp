#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dlib/image_processing.h>

using namespace std;

#define SPACE_KEY_CODE   32
#define ESCAPE_KEY_CODE  27

enum ErrorType
{
    ER_LOAD_FACE_MODULE = 0,
    ER_OPEN_WEBCAM,
    ER_OPEN_CELEBRITY_IMAGE
};

const char *error_msgs[] = 
{
    "Can't load face module",
    "Can't open webcam",
    "Can't load celebrity image"
};

std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
std::string face_shape_path = "shape_predictor_68_face_landmarks.dat";
std::string window_name = "faceOff";
std::string brad_pitt_path = "brad_pitt.bmp";

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

void draw_landmarks(cv::Mat &image, const std::vector<cv::Point> &landmarks)
{
    cv::Scalar blue_color = cv::Scalar(255, 0, 0);
    for (int i = 0; i < (int)landmarks.size(); i++)
    {
        cv::circle(
            image, landmarks[i],
            3, blue_color);
    }
}

void detect_landmarks(dlib::shape_predictor &face_shape_predictor, 
                      cv::Mat &image, const cv::Rect &face_rect, 
                      std::vector<cv::Point> &face_landmarks)
{
    int left   = face_rect.x;
    int top    = face_rect.y;
    int right  = face_rect.x + face_rect.width;
    int bottom = face_rect.y + face_rect.height;
    dlib::rectangle dlib_face = dlib::rectangle(left, top, right, bottom);

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

    int total_landmark = shape.num_parts();
    face_landmarks.resize(total_landmark);
    for (int i = 0; i < total_landmark; i++)
    {
        dlib::point landmark = shape.part(i);
        face_landmarks[i] = cv::Point(landmark.x(), landmark.y());
    }

    draw_landmarks(image, face_landmarks);
}

void draw_triangles(cv::Mat& img, const std::vector<cv::Vec6f> &triangles, cv::Scalar delaunay_color)
{
    using namespace cv;
 
    Size size = img.size();
    Rect rect(0, 0, size.width, size.height);

    vector<Point> pt(3);
    for (size_t i = 0; i < triangles.size(); i++)
    {
        const Vec6f &t = triangles[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

        // Draw rectangles completely inside the image.
        if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
        }
    }
}

void warp_image_by_trangles(
    cv::Mat &src_image, const std::vector<cv::Point2f> &src_triangles,
    cv::Mat &dst_image, const std::vector<cv::Point2f> &dst_triangles
    )
{

}

void get_triangles(cv::Mat &image, const std::vector<cv::Point> &landmarks, std::vector<cv::Vec6f> &triangles)
{
    using namespace cv;

    Subdiv2D subdiv(Rect(0, 0, image.cols, image.rows));
    for (int i = 0; i < (int)landmarks.size(); i++)
        subdiv.insert(Point2f(landmarks[i].x, landmarks[i].y));

    subdiv.getTriangleList(triangles);
}

void control_faces(
    cv::Mat &render_image,
    cv::Mat &src_image, const std::vector<cv::Point> &src_landmarks,
    cv::Mat &dst_image, const std::vector<cv::Point> &dst_landmarks)
{
    using namespace cv;

    int width = src_image.cols;
    int height = src_image.rows;

    std::vector<Vec6f> src_triangles;
    get_triangles(src_image, src_landmarks, src_triangles);

    std::vector<Vec6f> dst_triangles;
    get_triangles(dst_image, dst_landmarks, dst_triangles);

    Mat control_image = Mat(src_image.size(), CV_8UC3, Scalar(0));

    draw_triangles(src_image, src_triangles, cvScalar(255, 255, 255));
}

cv::Mat load_celebrity_info(
    const char *path, 
    cv::Rect &face_rect, std::vector<cv::Point> &face_landmark, 
    cv::CascadeClassifier &face_cascade, dlib::shape_predictor &face_shape_predictor)
{
    cv::Mat celebrity_img;
    celebrity_img = cv::imread(path);
    if (celebrity_img.empty())
        return celebrity_img;

    cv::Mat gray_celebrity_img;
    cv::cvtColor(celebrity_img, gray_celebrity_img, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    detect_faces(face_cascade, gray_celebrity_img, faces);
    if (!faces.empty())
    {
        face_rect = faces[0];
        detect_landmarks(face_shape_predictor, celebrity_img, face_rect, face_landmark);
    }   
    else
    {
        celebrity_img.release();
    }

    return celebrity_img;
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

    std::vector<cv::Point> brad_pitt_landmarks;
    cv::Rect brad_pitt_face;
    cv::Mat brad_pitt_img = load_celebrity_info(brad_pitt_path.c_str(), brad_pitt_face, brad_pitt_landmarks, face_cascade, face_shape_predictor);
    if (brad_pitt_img.empty())
        return error_happened(ER_OPEN_CELEBRITY_IMAGE);

    cv::namedWindow(window_name, 1);
    std::vector<cv::Rect> faces;
    bool is_in_morphing_mode = false;
    for (;;)
    {
        cv::Mat frame;
        cap >> frame;

        int width = frame.cols;
        int height = frame.rows;

        cv::Mat image;
        cv::flip(frame, image, 1);
        cv::Mat display_image = image;

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
        {
            cv::Mat render_image(cv::Size(width * 2, height), CV_8UC3, cv::Scalar(0));
            if (!faces.empty())
            {
                cv::Rect face_rect = faces[0];
                std::vector<cv::Point> face_landmarks;
                detect_landmarks(face_shape_predictor, image, face_rect, face_landmarks);
                control_faces(
                    render_image,
                    image, face_landmarks, 
                    brad_pitt_img, brad_pitt_landmarks);
            }
            image.copyTo(render_image(cv::Rect(0, 0, width, height)));
            display_image = render_image;
        }

        draw_faces(display_image, faces);
        cv::imshow(window_name, display_image);
        
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