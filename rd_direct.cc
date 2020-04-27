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
Matrix4D currXform;
Matrix4D world_to_cam;
Matrix4D cam_to_clip;
Matrix4D clip_to_device;
double cam_fov = 90.0;
double near_clip = 1.0;
double far_clip = 1000000000.0;
Point cam_eye = Point(0,0,0);
Point cam_look_at = Point(0,0,-1);
Vector3D cam_up = Vector3D(0,1,0);
PointH point_store; 

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
    currXform = currXform.identity();
    world_to_cam = world_to_cam.world_to_camera(cam_eye, cam_look_at, cam_up);
    cam_to_clip = cam_to_clip.camera_to_clip(cam_fov, near_clip, far_clip, (display_xSize / display_ySize));
    clip_to_device = clip_to_device.clip_to_device(display_xSize, display_ySize);
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
    //return rd_write_pixel(p[0], p[1], redgreenblue); // OLD ROUTINE

    PointH ph;
    ph[0] = p[0];
    ph[1] = p[1];
    ph[2] = p[2];
    ph[3] = 1;
    point_pipeline(ph);
    return RD_OK;
}

int REDirect::rd_line(const float start[3], const float end[3])
{
    //line(start, end); // OLD ROUTINE
    
    PointH ph;
    ph[0] = start[0];
    ph[1] = start[1];
    ph[2] = start[2];
    ph[3] = 1;

    PointH ph2;
    ph2[0] = end[0];
    ph2[1] = end[1];
    ph2[2] = end[2];
    ph2[3] = 1;

    line_pipeline(ph, false);
    line_pipeline(ph2, true);
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

int REDirect::rd_translate(const float offset[3])
{
    Matrix4D trans_matrix;
    Point p;
    p[0] = offset[0];
    p[1] = offset[1];
    p[2] = offset[2];
    trans_matrix.SetTranslation(p);
    currXform = Matrix_Matrix_Multiply(currXform, trans_matrix);
    return RD_OK;
}
int REDirect::rd_scale(const float scale_factor[3])
{
    Matrix4D scale_matrix;
    scale_matrix = scale_matrix.MakeScale(scale_factor[0], scale_factor[1], scale_factor[2]);
    currXform = Matrix_Matrix_Multiply(currXform, scale_matrix);
    return RD_OK;
}
int REDirect::rd_rotate_xy(float angle)
{
    Matrix4D xy_matrix;
    xy_matrix = xy_matrix.MakeRotationZ(angle);
    currXform = Matrix_Matrix_Multiply(currXform, xy_matrix);
    return RD_OK;
}
int REDirect::rd_rotate_yz(float angle)
{
    Matrix4D yz_matrix;
    yz_matrix = yz_matrix.MakeRotationX(angle);
    currXform = Matrix_Matrix_Multiply(currXform, yz_matrix);
    return RD_OK;
}
int REDirect::rd_rotate_zx(float angle)
{
    Matrix4D zx_matrix;
    zx_matrix = zx_matrix.MakeRotationY(angle);
    currXform = Matrix_Matrix_Multiply(currXform, zx_matrix);
    return RD_OK;
}
  
int REDirect::rd_xform_push(void)
{
    xforms.push_back(currXform);
    return RD_OK;
}
int REDirect::rd_xform_pop(void)
{
    currXform = xforms.back();
    xforms.pop_back();
    return RD_OK;
}


int REDirect::rd_camera_eye(const float eyepoint[3])
{
    cam_eye[0] = eyepoint[0];
    cam_eye[1] = eyepoint[1];
    cam_eye[2] = eyepoint[2];
    return RD_OK;
}
int REDirect::rd_camera_at(const float atpoint[3])
{
    cam_look_at[0] = atpoint[0];
    cam_look_at[1] = atpoint[1];
    cam_look_at[2] = atpoint[2];
    return RD_OK;
}
int REDirect::rd_camera_up(const float up[3])
{
    cam_up[0] = up[0];
    cam_up[1] = up[1];
    cam_up[2] = up[2];
    return RD_OK;
}
int REDirect::rd_camera_fov(float fov)
{
    cam_fov = fov;
}
int REDirect::rd_clipping(float znear, float zfar)
{
    near_clip = znear;
    far_clip = zfar;
}

int REDirect::rd_pointset(const string & vertex_type, int nvertex, const float * vertex)
{
    for (int i = 0; i < nvertex; i++)
    {
        PointH p;
        
        point_pipeline(p);
    }
}
int REDirect::rd_polyset(const string & vertex_type, int nvertex, const float * vertex, int nface, const int * face)
{

}

int REDirect::point_pipeline(PointH& ph)
{
    ph = Matrix_PointH_Multiply(currXform, ph);
    ph = Matrix_PointH_Multiply(world_to_cam, ph);
    ph = Matrix_PointH_Multiply(cam_to_clip, ph);
    if (ph[0] >= 0 && ph[0] <= 1 && ph[1] >= 0 && ph[1] <= 1 && ph[2] >= 0 && ph[2] <= 1)
    {
        ph = Matrix_PointH_Multiply(clip_to_device, ph);
        rd_write_pixel(ph[0], ph[1], redgreenblue);
    }
    return RD_OK;
}

int REDirect::line_pipeline(PointH ph, bool draw)
{
    if (point_pipeline(ph)) rd_write_pixel(ph[0], ph[1], redgreenblue);
    if (!draw) point_store = ph;
    else if (draw)
    {
        point_store = Matrix_PointH_Multiply(clip_to_device, point_store);
        ph = Matrix_PointH_Multiply(clip_to_device, ph);
        float start[3];
        float end[3];
        start[0] = point_store[0];
        start[1] = point_store[1];
        start[2] = point_store[2];
        end[0] = point_store[0];
        end[1] = point_store[1];
        end[2] = point_store[2];
        line(start, end);
        point_store = ph;
    } 
    return RD_OK;
}

int REDirect::rd_cube(void)
{
    PointH p1 = PointH(-1, -1, -1, 1);
    PointH p2 = PointH(1, -1, -1, 1);
    PointH p3 = PointH(1, 1, -1, 1);
    PointH p4 = PointH(-1, 1, -1, 1);
    PointH p5 = PointH(-1, -1, 1, 1);
    PointH p6 = PointH(1, -1, 1, 1);
    PointH p7 = PointH(1, 1, 1, 1);
    PointH p8 = PointH(-1, 1, 1, 1);
    line_pipeline(p1, false);
    line_pipeline(p2, true);
    line_pipeline(p3, true);
    line_pipeline(p4, true);
    line_pipeline(p1, true);
    line_pipeline(p5, true);
    line_pipeline(p6, true);
    line_pipeline(p7, true);
    line_pipeline(p8, true);
    line_pipeline(p5, true);

}

int REDirect::rd_disk(float height, float radius, float theta)
{
    std::cout << theta << std::endl;
}

int REDirect::rd_cylinder(float radius, float zmin, float zmax, float thetamax)
{

}

int REDirect::rd_cone(float height, float radius, float thetamax)
{
    rd_disk(height, radius, thetamax);
}


int REDirect::rd_sphere(float radius, float zmin, float zmax, float thetamax)
{

}