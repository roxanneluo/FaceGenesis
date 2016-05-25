#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

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
CascadeClassifier face_cascade;
string window_name = "faceOff";
RNG rng(2046);

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

void detect_face(const Mat &gray_image, vector<Rect> &faces)
{
    faces.clear();

    equalizeHist(gray_image, gray_image);
    face_cascade.detectMultiScale(gray_image, faces, 1.21, 3, 0, Size(30, 30));
}

int main(int, char**)
{
    if (!face_cascade.load(face_cascade_name))
        return error_happened(ER_LOAD_FACE_MODULE);

    VideoCapture cap(0); 
    if (!cap.isOpened())
        return error_happened(ER_OPEN_WEBCAM);

    namedWindow(window_name, 1);
    for (;;)
    {
        Mat image;
        cap >> image;
        flip(image, image, 1);

        Mat gray_image;
        cvtColor(image, gray_image, COLOR_BGR2GRAY);

        vector<Rect> faces;
        detect_face(gray_image, faces);
        draw_faces(image, faces);

        imshow(window_name, image);
        
        if (waitKey(30) >= 0) 
            break;
    }

    return 0;
}