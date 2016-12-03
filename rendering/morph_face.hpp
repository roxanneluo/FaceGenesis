#include <iostream>
#include <vector>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;

#define SPACE_KEY_CODE   0x20
#define ESCAPE_KEY_CODE  0x1B
#define N_KEY_CODE       0x6E
#define R_KEY_CODE       0x72
#define L_KEY_CODE       0x6C
#define T_KEY_CODE       0x74
#define S_KEY_CODE       0x73

int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
double fontScale = 1;

bool g_is_show_lanmarks = false;
bool g_is_show_trangles = false;

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

bool g_is_tracker_init = false;
std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
std::string face_shape_path = "shape_predictor_68_face_landmarks.dat";
std::string window_name = "[ESC]:Quit, [space]:Enter/Leave Morphing, [n]:Next Celebrity, [r]: Re-detect Face, [l]: show landmarkds, [t]: show triangles, [s]: save image";

const char* celebrity_paths[] =
{
    "chris.jpg",
    "brad_pitt.bmp",
    "obama.bmp",
    "hillary.bmp",
    "trump.bmp",
    "tom_hanks.bmp",
};

int error_happened(const ErrorType error_type)
{
    cout << error_msgs[error_type] << endl;
    return -1;
}

bool is_mouth_seam_by_idx(const int index)
{
    return (index >= 60);
}

class MySubdiv2D : public cv::Subdiv2D
{
public:
    MySubdiv2D(cv::Rect rect)
        :cv::Subdiv2D(rect)
    {

    }

    // skips "outer" triangles.
    void getTriangleIndices(vector<int> &indices) const
    {
        using namespace cv;

        int i, total = (int)(qedges.size() * 4);
        std::vector<bool> edgemask(total, false);

        for (i = 4; i < total; i += 2)
        {
            if (edgemask[i])
                continue;

            int index[3];
            Point2f a, b, c;
            int edge = i;
            
            index[0] = edgeOrg(edge, &a);
            if (index[0] < 4)
                continue;
            edgemask[edge] = true;
            
            edge = getEdge(edge, NEXT_AROUND_LEFT);
            index[1] = edgeOrg(edge, &b);
            if (index[1] < 4)
                continue;
            edgemask[edge] = true;
            
            edge = getEdge(edge, NEXT_AROUND_LEFT);
            index[2] = edgeOrg(edge, &c);
            if (index[2] < 4)
                continue;
            edgemask[edge] = true;

            indices.push_back(index[0]-4);
            indices.push_back(index[1]-4);
            indices.push_back(index[2]-4);
        }
    }
};

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

    std::vector<Rect> detected_faces;
    equalizeHist(gray_image, gray_image);
    face_cascade.detectMultiScale(gray_image, detected_faces, 1.21, 3, 0, Size(30, 30));

    if (detected_faces.size() > 1)
    {
        int max_area = INT_MIN;
        int max_face_index = -1;

        for (int i = 0; i < (int)detected_faces.size(); i++)
        {
            int face_area = detected_faces[i].width * detected_faces[i].height;
            if (face_area > max_area)
            {
                max_area = face_area;
                max_face_index = i;
            }
        }
        assert(max_face_index != -1);

        faces.push_back(detected_faces[max_face_index]);
    }
    else
    {
        faces = detected_faces;
    }
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

void draw_triangles(cv::Mat& img, const std::vector<cv::Vec6f> &triangles)
{
    using namespace cv;
 
    Scalar white_color = Scalar(255, 255, 255);

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
            line(img, pt[0], pt[1], white_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], white_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], white_color, 1, CV_AA, 0);
        }
    }
}

void warp_image_by_trangles(
    cv::Mat &src_image, const cv::Vec6f &src_triangle,
    cv::Mat &dst_image, const cv::Vec6f &dst_triangle
    )
{
    using namespace cv;

    std::vector<Point2f> src_pts;
    std::vector<Point2f> dst_pts;
    for (int i = 0; i < 3; i++)
    {
        src_pts.push_back(Point2f(src_triangle[i * 2], src_triangle[i * 2 + 1]));
        dst_pts.push_back(Point2f(dst_triangle[i * 2], dst_triangle[i * 2 + 1]));
    }
    Rect src_roi = boundingRect(src_pts);
    Rect dst_roi = boundingRect(dst_pts);

    std::vector<Point2f> shift_src_pts;
    std::vector<Point2f> shift_dst_pts;
    for (int i = 0; i < 3; i++)
    {
        shift_src_pts.push_back(Point2f(src_pts[i].x - src_roi.x, src_pts[i].y - src_roi.y));
        shift_dst_pts.push_back(Point2f(dst_pts[i].x - dst_roi.x, dst_pts[i].y - dst_roi.y));
    }

    Mat affine_matrix = getAffineTransform(shift_src_pts, shift_dst_pts);
        
    Mat src_roi_image;
    src_image(src_roi).copyTo(src_roi_image);
    Mat dst_roi_image(Size(dst_roi.width, dst_roi.height), CV_8UC3, Scalar(0));
    warpAffine(src_roi_image, dst_roi_image, affine_matrix, dst_roi_image.size(), INTER_LINEAR, BORDER_CONSTANT);

    Mat mask(Size(dst_roi.width, dst_roi.height), CV_8UC1, Scalar(0));
    std::vector<Point> shift_dst_ptsI;
    for (int i = 0; i < 3; i++)
        shift_dst_ptsI.push_back(Point(shift_dst_pts[i].x, shift_dst_pts[i].y));
    fillConvexPoly(mask, shift_dst_ptsI, Scalar(255));
    dst_roi_image.copyTo(dst_image(dst_roi), mask);
}

void get_triangles(
    cv::Mat &image, const std::vector<cv::Point> &landmarks, 
    std::vector<cv::Vec6f> &triangles, std::vector<int> &triangle_indices)
{
    using namespace cv;

    MySubdiv2D subdiv(Rect(0, 0, image.cols, image.rows));
    for (int i = 0; i < (int)landmarks.size(); i++)
        subdiv.insert(Point2f(landmarks[i].x, landmarks[i].y));

    subdiv.getTriangleIndices(triangle_indices);
    
    for (int i = 0; i < (int)triangle_indices.size(); i += 3)
    {
        Vec6f t;
        t[0] = landmarks[triangle_indices[i]].x;
        t[1] = landmarks[triangle_indices[i]].y;
        t[2] = landmarks[triangle_indices[i + 1]].x;
        t[3] = landmarks[triangle_indices[i + 1]].y;
        t[4] = landmarks[triangle_indices[i + 2]].x;
        t[5] = landmarks[triangle_indices[i + 2]].y;
        triangles.push_back(t);
    }
}

cv::Mat control_faces(
    cv::Mat &src_image, const std::vector<cv::Point> &src_landmarks,
    cv::Mat &dst_image, const std::vector<cv::Point> &dst_landmarks,
    bool is_remove_mouth_seam_region)
{
    using namespace cv;

    int width = src_image.cols;
    int height = src_image.rows;

    std::vector<Vec6f> dst_triangles;
    std::vector<int> dst_triangle_indices;
    get_triangles(dst_image, dst_landmarks, dst_triangles, dst_triangle_indices);
    cout << dst_triangles.size();

    std::vector<Vec6f> src_triangles;
    for (int i = 0; i < (int)dst_triangle_indices.size(); i+=3)
    {
        Vec6f t;
        t[0] = src_landmarks[dst_triangle_indices[i]].x;
        t[1] = src_landmarks[dst_triangle_indices[i]].y;
        t[2] = src_landmarks[dst_triangle_indices[i+1]].x;
        t[3] = src_landmarks[dst_triangle_indices[i+1]].y;
        t[4] = src_landmarks[dst_triangle_indices[i+2]].x;
        t[5] = src_landmarks[dst_triangle_indices[i+2]].y;
        src_triangles.push_back(t);
    }

    assert(src_triangles.size() == dst_triangles.size());

    Mat control_image = Mat(src_image.size(), CV_8UC3, Scalar(0));
    for (size_t i = 0; i < src_triangles.size(); i++)
        warp_image_by_trangles(dst_image, dst_triangles[i], control_image, src_triangles[i]);

    if (is_remove_mouth_seam_region)
    {
        int max_y = INT_MIN;
        int min_y = INT_MAX;
        int max_x = INT_MIN;
        int min_x = INT_MAX;
        std::vector<cv::Point> mouth_seam_pts;
        for (int i = 0; i < (int)src_landmarks.size(); i++)
        {
            if (is_mouth_seam_by_idx(i))
            {
                cv::Point pt = src_landmarks[i];
                mouth_seam_pts.push_back(pt);

                max_y = std::max(pt.y, max_y);
                min_y = std::min(pt.y, min_y);
                max_x = std::max(pt.x, max_x);
                min_x = std::min(pt.x, min_x);
            } 
        }
        if ((max_y - min_y)/(float)(max_x - min_x) > 0.05f) // if the user open his/her mouth large enough 
            fillConvexPoly(control_image, mouth_seam_pts, Scalar(0, 0, 0));
    }

    if (g_is_show_trangles)
    {
        draw_triangles(control_image, src_triangles);
        draw_triangles(src_image, src_triangles);
    }
        
    if (g_is_show_lanmarks)
        draw_landmarks(control_image, src_landmarks);

    return control_image;
}
