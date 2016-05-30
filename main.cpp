#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dlib/image_processing.h>

using namespace std;

#define SPACE_KEY_CODE   0x20
#define ESCAPE_KEY_CODE  0x1B
#define N_KEY_CODE       0x6E

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

const char* celebrity_paths[] =
{
    "brad_pitt.bmp",
    "obama.bmp",
    "tom_hanks.bmp"
};

std::string brad_pitt_path = "brad_pitt.bmp";

int error_happened(const ErrorType error_type)
{
    cout << error_msgs[error_type] << endl;
    return -1;
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
    {
        shift_dst_ptsI.push_back(Point(shift_dst_pts[i].x, shift_dst_pts[i].y));
    }
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
    cv::Mat &dst_image, const std::vector<cv::Point> &dst_landmarks)
{
    using namespace cv;

    int width = src_image.cols;
    int height = src_image.rows;

    std::vector<Vec6f> dst_triangles;
    std::vector<int> dst_triangle_indices;
    get_triangles(dst_image, dst_landmarks, dst_triangles, dst_triangle_indices);

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

    return control_image;
}

struct CelebrityInfo
{
    cv::Mat image;
    cv::Rect face_rect;
    std::vector<cv::Point> landmarks;
};

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

class CelebrityMorpher
{
public:
    CelebrityMorpher()
    {
        m_mode = SELF_PORTRAINT;
        m_celebrity_amount = 0;
        m_celebrity_index = 0;

        m_is_in_animation = false;
    }
    ~CelebrityMorpher(){}

    bool init(cv::CascadeClassifier &face_cascade, dlib::shape_predictor &face_shape_predictor)
    {
        bool is_init_ok = true;

        int celebrity_amount = sizeof(celebrity_paths) / sizeof(const char*);
        for (int i = 0; i < celebrity_amount; i++)
        {
            m_celebrities.push_back(CelebrityInfo());
            CelebrityInfo &info = m_celebrities.back();
            info.image = load_celebrity_info(
                celebrity_paths[i], 
                info.face_rect, info.landmarks, 
                face_cascade, face_shape_predictor);
            if (info.image.empty())
            {
                is_init_ok = false;
                break;
            }
        }

        if (is_init_ok)
            m_celebrity_amount = celebrity_amount;

        return is_init_ok;
    }

    void start_session()
    {
        m_mode = SELF_PORTRAINT;
        m_celebrity_index = 0;

        m_is_in_animation = false;
    }
    void end_session()
    {
        if (m_is_in_animation)
            end_animation();
    }

    cv::Mat get_image(
        cv::Mat &render_image,
        cv::Mat &src_image, const cv::Rect &src_face_rect,
        dlib::shape_predictor &face_shape_predictor)
    {
        if (m_is_in_animation)
            return get_animation_frame();

        src_image.copyTo(m_user_image);

        m_user_face_landmarks.clear();
        detect_landmarks(face_shape_predictor, m_user_image, src_face_rect, m_user_face_landmarks);

        switch (m_mode)
        {
        case SELF_PORTRAINT:
            return control_faces(m_user_image, m_user_face_landmarks, m_user_image, m_user_face_landmarks);

        case PUPPETRY:
            return control_faces(
                m_user_image, m_user_face_landmarks,
                m_celebrities[m_celebrity_index].image, m_celebrities[m_celebrity_index].landmarks);

        default:
            assert(false);
            break;
        }

        return cv::Mat();
    }

    void next()
    {
        if (m_is_in_animation)
            return;

        switch (m_mode)
        {
        case SELF_PORTRAINT:
            m_mode = PUPPETRY;
            m_celebrity_index = 0;
            start_animation();
            break;

        case PUPPETRY:
            m_celebrity_index++;
            if (m_celebrity_index >= m_celebrity_amount)
            {
                m_celebrity_index = 0;
                m_mode = SELF_PORTRAINT;
            }
            start_animation();
            break;

        default:
            assert(false);
            break;
        }
    }

protected:
    enum
    {
        SELF_PORTRAINT = 0,
        PUPPETRY
    } m_mode;

    cv::Mat m_animate_start_image;
    cv::Mat m_animate_end_image;

    cv::Mat m_user_image;
    std::vector<cv::Point> m_user_face_landmarks;
    
    std::vector<CelebrityInfo> m_celebrities;
    int m_celebrity_amount;
    int m_celebrity_index;

    bool m_is_in_animation;
    int m_animation_counter;
    static int ANIMATE_FRAME_AMOUNT;

    void start_animation()
    {
        m_is_in_animation = true;
        m_animation_counter = 0;
        switch (m_mode)
        {
        case SELF_PORTRAINT:
            m_animate_start_image = control_faces(m_user_image, m_user_face_landmarks, m_celebrities[m_celebrity_amount - 1].image, m_celebrities[m_celebrity_amount - 1].landmarks);
            m_animate_end_image = control_faces(m_user_image, m_user_face_landmarks, m_user_image, m_user_face_landmarks);
            break;

        case PUPPETRY:
            if (m_celebrity_index > 0)
            {
                m_animate_start_image = control_faces(m_user_image, m_user_face_landmarks, m_celebrities[m_celebrity_index - 1].image, m_celebrities[m_celebrity_index - 1].landmarks);
                m_animate_end_image = control_faces(m_user_image, m_user_face_landmarks, m_celebrities[m_celebrity_index].image, m_celebrities[m_celebrity_index].landmarks);
            }
            else
            {
                assert(m_celebrity_index == 0);
                m_animate_start_image = control_faces(m_user_image, m_user_face_landmarks, m_user_image, m_user_face_landmarks);
                m_animate_end_image = control_faces(m_user_image, m_user_face_landmarks, m_celebrities[0].image, m_celebrities[0].landmarks);
            }
            break;

        default:
            break;
        }
    }
    void end_animation()
    {
        m_is_in_animation = false;
        m_animation_counter = 0;
        m_animate_start_image.release();
        m_animate_end_image.release();
    }

    cv::Mat get_animation_frame()
    {
        double ratio = (double)m_animation_counter / ANIMATE_FRAME_AMOUNT;

        cv::Mat animatation_frame;
        cv::addWeighted(m_animate_start_image, 1.0 - ratio, m_animate_end_image, ratio, 0, animatation_frame);

        m_animation_counter++;
        if (m_animation_counter > ANIMATE_FRAME_AMOUNT)
            end_animation();

        return animatation_frame;
    }
};
int CelebrityMorpher::ANIMATE_FRAME_AMOUNT = 60;


int main(int, char**)
{
    cout << "load face detector ..." << endl;
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_name))
        return error_happened(ER_LOAD_FACE_MODULE);

    cout << "load face landmark detector ..." << endl;
    dlib::shape_predictor face_shape_predictor;
    dlib::deserialize(face_shape_path.c_str()) >> face_shape_predictor;

    cout << "open video camera ..." << endl;
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return error_happened(ER_OPEN_WEBCAM);

    cout << "load celebrity models ..." << endl;
    CelebrityMorpher celebrity_morpher;
    if (!celebrity_morpher.init(face_cascade, face_shape_predictor))
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

        bool is_has_face = false;
        if (is_in_morphing_mode)
        {
            cv::Mat render_image(cv::Size(width * 2, height), CV_8UC3, cv::Scalar(0));
            is_has_face = !faces.empty();
            if (is_has_face)
            {
                cv::Mat morph_image = celebrity_morpher.get_image(render_image, image, faces[0], face_shape_predictor);
                morph_image.copyTo(render_image(cv::Rect(width, 0, width, height)));
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

        switch (key_code)
        {
        case SPACE_KEY_CODE:
            if (!is_in_morphing_mode)
            {
                celebrity_morpher.start_session();
                is_in_morphing_mode = true;
            }
            else
            {
                celebrity_morpher.end_session();
                is_in_morphing_mode = false;
            }
            break;

        case N_KEY_CODE:
            if ((is_in_morphing_mode) && (is_has_face))
                celebrity_morpher.next();
            break;

        default:
            break;
        }
    }

    return 0;
}