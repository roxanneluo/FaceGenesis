#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
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

String face_cascade_name = "haarcascade_frontalface_alt.xml";
string window_name = "faceOff";

int error_happened(const ErrorType error_type)
{
    cout << error_msgs[error_type] << endl;
    return -1;
}

void draw_faces(const Mat &image, vector<Rect> &faces)
{
    Scalar red_color = Scalar(0, 0, 255);
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        ellipse(image, center, 
                Size(faces[i].width / 2, faces[i].height / 2),
                0, 0, 360, red_color, 4);
    }
}

void detect_faces(CascadeClassifier *p_face_cascade, const Mat &gray_image, vector<Rect> &faces)
{
    faces.clear();

    equalizeHist(gray_image, gray_image);
    p_face_cascade->detectMultiScale(gray_image, faces, 1.21, 3, 0, Size(30, 30));
}

int track_faces(vector<Rect> &faces)
{
    // do face tracking here
    // return number of succeeded tracked faces
    
    // temporally, we clear all detected faces here
    // the code should be removed when a face tracking algorithm is implemented.
    faces.clear();

    return (int)faces.size();
}

void morph_faces(Mat &image, const vector<Rect> &faces)
{
    cout << "morph face" << endl;
}

int main(int, char**)
{
    CascadeClassifier *p_face_cascade = new CascadeClassifier();

    if (!p_face_cascade->load(face_cascade_name))
        return error_happened(ER_LOAD_FACE_MODULE);

    VideoCapture cap(0); 
    if (!cap.isOpened())
        return error_happened(ER_OPEN_WEBCAM);

    namedWindow(window_name, 1);
    vector<Rect> faces;
    bool is_in_morphing_mode = false;
    for (;;)
    {
        Mat frame;
        cap >> frame;

        Mat image;
        flip(frame, image, 1);

        Mat gray_image;
        cvtColor(image, gray_image, COLOR_BGR2GRAY);

        if (faces.empty())
        {
            detect_faces(p_face_cascade, gray_image, faces);
        } 
        else
        {
            if (track_faces(faces) <= 0)
            {
                // when tracking failed, re-detect faces
                detect_faces(p_face_cascade, gray_image, faces);
            }
        }
     
        if (is_in_morphing_mode)
            morph_faces(image, faces);

        draw_faces(image, faces);

        imshow(window_name, image);
        
        int key_code = waitKey(10);
        bool is_leaving = (key_code == ESCAPE_KEY_CODE);
        if (is_leaving)
            break;

        // enter / leave morphing mode
        if (key_code == SPACE_KEY_CODE)
            is_in_morphing_mode = !is_in_morphing_mode; 
    }

    delete p_face_cascade;

    return 0;
}