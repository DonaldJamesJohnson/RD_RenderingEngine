#include "rd_direct.h"
#include "rd_error.h"
#include "rd_display.h"
#include "iostream"
#include "Data_Structures/Vector3D.h"
#include "Data_Structures/Point.h"
#include "Data_Structures/PointH.h"
#include "Data_Structures/Matrix4D.h"
#include "vector"
using std::vector;

int frameNumber;
float redgreenblue[3] = {1.0, 1.0, 1.0};
vector<Matrix4D> xforms;
vector<Matrix4D> currXform;
Matrix4D world_to_clip;
Matrix4D clip_to_device;
double cam_fov = 90.0;
double near_clip = 1.0;
double far_clip = 1000000000.0;
Point cam_eye = Point(0,0,0);
Point cam_look_at = Point(0,0,-1);
Point cam_up = Point(0,1,0); 

int REDirect::rd_display(const string & name, const string & type, const string & mode)
{
    return RD_OK;
}

int REDirect::rd_format(int xresolution, int yresolution)
{
    return RD_OK;
}

int REDirect::rd_world_begin(void)
{
    rd_disp_init_frame(frameNumber);
    return RD_OK;
}
int REDirect::rd_world_end(void)
{
    rd_disp_end_frame();
    return RD_OK;
}

int REDirect::rd_frame_begin(int frame_no)
{
    frameNumber = frame_no;
    return RD_OK;
}

int REDirect::rd_frame_end(void)
{
    rd_disp_end_frame();
    return RD_OK;
}

int REDirect::rd_render_init(void)
{
    return RD_OK;
}

int REDirect::rd_render_cleanup(void)
{
    return RD_OK;
}

int REDirect::rd_color(const float color[])
{
    redgreenblue[0] = color[0];
    redgreenblue[1] = color[1];
    redgreenblue[2] = color[2];
    return RD_OK;
}

int REDirect::rd_background(const float color[])
{
    return rd_set_background(color);
}

int REDirect::rd_point(const float p[3])
{
    return rd_write_pixel(p[0], p[1], redgreenblue);
}

int REDirect::rd_line(const float start[3], const float end[3])
{
    line(start, end);
    return RD_OK;
}

int REDirect::rd_circle(const float center[3], float radius)
{
    circle(center, radius);
    return RD_OK;
}

int REDirect::rd_fill(const float seed_point[3]) //xyz 
{
    // use find span to determine xs and xe
    // look at seed point color and set seed_color to that
    float seed_color[3];
    rd_read_pixel(seed_point[0], seed_point[1], seed_color);
    int x = seed_point[0];
    int y = seed_point[1];
    fill(x, y, seed_color, redgreenblue);
    /*
    int xs = seed_point[0];
    int xe = xs+1;
    int y = seed_point[1];
    //findSpan(xs, xe, y, seed_color);
    //std::cout << "xs: " << xs << " | xe: " << xe << std::endl;
    fill(xs, xe, y, seed_color);
    */
    return RD_OK;
}

void REDirect::line(const float start[3], const float end[3])
{
    float x;
    float y;
    float xs;
    float ys;
    float xe;
    float ye;
    float dx;
    float dy;
    float p;
    float plot[3];
    // Initialize the delta x and y values
    if (start[0] < end[0]) dx = end[0] - start[0];
    else dx = start[0] - end[0];
    if (start[1] < end[1]) dy = end[1] - start[1];
    else dy = start[1] - end[1];

    // Set p based on whether delta x or y is greater
    if (dy < dx) p = (2 * (dy)) - (dx);
    else p = (2 * (dx)) - (dy);

    if (start[0] < end[0] && start[1] <= end[1] && dx > dy)        // Case 1
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        y = ys;
        for (x = xs; x <= xe; x++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p >= 0)
            {
                p += (2 * dy) - (2 * dx);
                y++;
            }
            else 
            {
                p += (2 * dy);
            }
        }
    }
    else if (start[0] < end[0] && start[1] < end[1] && dx <= dy)    // Case 2
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        x = xs;
        for (y = ys; y <= ye; y++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p > 0)
            {
                p += (2 * dx) - (2 * dy);
                x++;
            }
            else 
            {
                p += (2 * dx);
            }
        }
    }
    else if (start[0] >= end[0] && start[1] < end[1] && dx < dy)   // Case 3
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        x = xs;
        for (y = ys; y <= ye; y++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p > 0)
            {
                p += (2 * dx) - (2 * dy);
                x--;
            }
            else 
            {
                p += (2 * dx);
            }
        }
    }
    else if (start[0] > end[0] && start[1] < end[1] && dx >= dy)    // Case 4
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        y = ye;
        for (x = xe; x <= xs; x++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p >= 0)
            {
                p += (2 * dy) - (2 * dx);
                y--;
            }
            else 
            {
                p += (2 * dy);
            }
        }
    }
    else if (start[0] > end[0] && start[1] >= end[1] && dx > dy)   // Case 5
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        y = ye;
        for (x = xe; x <= xs; x++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p >= 0)
            {
                p += (2 * dy) - (2 * dx);
                y++;
            }
            else 
            {
                p += (2 * dy);
            }
        }
    }
    else if (start[0] > end[0] && start[1] > end[1] && dx <= dy)    // Case 6
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        x = xe;
        for (y = ye; y <= ys; y++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p > 0)
            {
                p += (2 * dx) - (2 * dy);
                x++;
            }
            else 
            {
                p += (2 * dx);
            }
        }
    }
    else if (start[0] <= end[0] && start[1] > end[1] && dx < dy)   // Case 7
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        x = xe;
        for (y = ye; y <= ys; y++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p > 0)
            {
                p += (2 * dx) - (2 * dy);
                x--;
            }
            else 
            {
                p += (2 * dx);
            }
        }
    }
    else if (start[0] < end[0] && start[1] > end[1] && dx >= dy)    // Case 8
    {
        xs = start[0];
        xe = end[0];
        ys = start[1];
        ye = end[1];
        y = ys;
        for (x = xs; x <= xe; x++)
        {
            plot[0] = x;
            plot[1] = y;
            rd_write_pixel(plot[0], plot[1], redgreenblue);
            if (p >= 0)
            {
                p += (2 * dy) - (2 * dx);
                y--;
            }
            else 
            {
                p += (2 * dy);
            }
        }
    }
}

void REDirect::circle(const float center[3], const float radius)
{
    
    float x = 0;
    float y = radius;
    float p = 3 - 2 * radius;
    rd_write_pixel(center[0]+x, center[1]+y, redgreenblue);
    rd_write_pixel(center[0]-x, center[1]+y, redgreenblue);
    rd_write_pixel(center[0]+x, center[1]-y, redgreenblue);
    rd_write_pixel(center[0]-x, center[1]-y, redgreenblue);
    rd_write_pixel(center[0]+y, center[1]+x, redgreenblue);
    rd_write_pixel(center[0]-y, center[1]+x, redgreenblue);
    rd_write_pixel(center[0]+y, center[1]-x, redgreenblue);
    rd_write_pixel(center[0]-y, center[1]-x, redgreenblue);
    while (x <= y){
        if (p > 0)
        {
            p += 4 * (x - y) + 10;
            x++;
            y--;
        }
        else 
        {
            p += 4 * x + 6;
            x++;
        }
        rd_write_pixel(center[0]+x, center[1]+y, redgreenblue);
        rd_write_pixel(center[0]-x, center[1]+y, redgreenblue);
        rd_write_pixel(center[0]+x, center[1]-y, redgreenblue);
        rd_write_pixel(center[0]-x, center[1]-y, redgreenblue);
        rd_write_pixel(center[0]+y, center[1]+x, redgreenblue);
        rd_write_pixel(center[0]-y, center[1]+x, redgreenblue);
        rd_write_pixel(center[0]+y, center[1]-x, redgreenblue);
        rd_write_pixel(center[0]-y, center[1]-x, redgreenblue);
    }
    
}

void REDirect::fill(int x, int y, float seed_color[3], float new_color[3])
{   
    float check_color[3];
    rd_read_pixel(x, y, check_color);
    if (check_color[0] == seed_color[0] && check_color[1] == seed_color[1] && check_color[2] == seed_color[2])
    {
        rd_write_pixel(x, y, redgreenblue);
        if (x < display_xSize - 1) fill(x + 1, y, seed_color, redgreenblue);
        if (x > 0) fill(x - 1, y, seed_color, redgreenblue);
        if (y < display_ySize - 1) fill(x, y + 1, seed_color, redgreenblue);
        if (y > 0) fill(x, y - 1, seed_color, redgreenblue);
    }
}

void push(const Matrix4D& m)
{
    xforms.push_back(m);
}

Matrix4D pop()
{
    Matrix4D m = xforms.back();
    xforms.pop_back();
    return m;
}

int REDirect::rd_translate(const float offset[3])
{

    return RD_OK;
}
int REDirect::rd_scale(const float scale_factor[3])
{
    return RD_OK;
}
int REDirect::rd_rotate_xy(float angle)
{
    return RD_OK;
}
int REDirect::rd_rotate_yz(float angle)
{
    return RD_OK;
}
int REDirect::rd_rotate_zx(float angle)
{
    return RD_OK;
}
int REDirect::rd_matrix(const float * mat)
{
    return RD_OK;
}
  
int REDirect::rd_xform_push(void)
{
    return RD_OK;
}
int REDirect::rd_xform_pop(void)
{
    pop();
    return RD_OK;
}

int point_pipeline(PointH ph)
{
    for (int i = 0; i < currXform.size; i++) ph = Matrix_PointH_Multiply(currXform[i], ph);
}

int line_pipeline(PointH ph, bool draw)
{

}
