#include "rd_direct.h"
#include "rd_error.h"
#include "rd_display.h"
#include "iostream"
#include "Data_Structures/Vector3D.h"
#include "Data_Structures/Point.h"
#include "Data_Structures/PointH.h"
#include "Data_Structures/Matrix4D.h"
#include <stack>
using std::stack;
using namespace std;


int frameNumber;
float redgreenblue[3] = {1.0, 1.0, 1.0};
stack<Matrix4D> xforms;
Matrix4D currXform;
Matrix4D world_to_cam;
Matrix4D cam_to_clip;
Matrix4D clip_to_device;
Matrix4D final_trans;
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
    cam_to_clip = cam_to_clip.camera_to_clip(cam_fov, near_clip, far_clip, display_xSize, display_ySize);
    clip_to_device = clip_to_device.clip_to_device(display_xSize, display_ySize);
    final_trans = Matrix4D(Matrix_Matrix_Multiply(clip_to_device, cam_to_clip));
    final_trans = Matrix_Matrix_Multiply(final_trans, world_to_cam);
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
    trans_matrix = trans_matrix.SetTranslation(p);    
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
    Matrix4D push_trans = currXform;
    xforms.push(push_trans);
    return RD_OK;
}
int REDirect::rd_xform_pop(void)
{
    currXform = xforms.top();
    xforms.pop();
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
    return RD_OK;
}
int REDirect::rd_clipping(float znear, float zfar)
{
    near_clip = znear;
    far_clip = zfar;
    return RD_OK;
}

int REDirect::rd_pointset(const string & vertex_type,int nvertex, const float * vertex) 
{ 
    int num_points = nvertex; 	
    float pn[3]; 	
    const float * vertices1 = vertex; 	
    for(int i = 0;i<num_points;i++) 	
    { 	    
        pn[0]=vertices1[i*3+0]; 		
        pn[1]=vertices1[i*3+1]; 		
        pn[2]=vertices1[i*3+2]; 		
        rd_point(pn); 	
        } 	
return RD_OK; 
}
int REDirect::rd_polyset(const string & vertex_type, int nvertex, const float * vertex, int nface, const int * face)
{
 return RD_OK;
}

int REDirect::point_pipeline(PointH& ph)
{
    std::cout << ph[0] << ", " << ph[1]  << ", " << ph[2]  << ", " << ph[3] << '\n' << std::endl;
    std::cout << "Current Transform" << std::endl;
    std::cout << currXform[0][0] << " " << currXform[0][1] << " " << currXform[0][2] << " " << currXform[0][3] << std::endl;
    std::cout << currXform[1][0] << " " << currXform[1][1] << " " << currXform[1][2] << " " << currXform[1][3] << std::endl;
    std::cout << currXform[2][0] << " " << currXform[2][1] << " " << currXform[2][2] << " " << currXform[2][3] << std::endl;
    std::cout << currXform[3][0] << " " << currXform[3][1] << " " << currXform[3][2] << " " << currXform[3][3] << '\n' << std::endl;
    PointH ph_trans;
    ph_trans = Matrix_PointH_Multiply(currXform, ph);
    std::cout << ph_trans[0] << ", " << ph_trans[1]  << ", " << ph_trans[2]  << ", " << ph_trans[3] << '\n' << std::endl;
    ph_trans[0] = ph_trans[0] / ph_trans[3];
    ph_trans[1] = ph_trans[1] / ph_trans[3];
    ph_trans[2] = ph_trans[2] / ph_trans[3];
    ph_trans[3] = 0;
    std::cout << "DIVIDE BY W: " << ph_trans[0] << ", " << ph_trans[1]  << ", " << ph_trans[2]  << ", " << ph_trans[3] << '\n' << std::endl;
    std::cout << "Final Transform" << std::endl;
    std::cout << final_trans[0][0] << " " << final_trans[0][1] << " " << final_trans[0][2] << " " << final_trans[0][3] << std::endl;
    std::cout << final_trans[1][0] << " " << final_trans[1][1] << " " << final_trans[1][2] << " " << final_trans[1][3] << std::endl;
    std::cout << final_trans[2][0] << " " << final_trans[2][1] << " " << final_trans[2][2] << " " << final_trans[2][3] << std::endl;
    std::cout << final_trans[3][0] << " " << final_trans[3][1] << " " << final_trans[3][2] << " " << final_trans[3][3] << '\n' << std::endl;
    PointH finalPoint;
    finalPoint = Matrix_PointH_Multiply(final_trans, ph_trans);
    std::cout << finalPoint[0] << ", " << finalPoint[1]  << ", " << finalPoint[2]  << ", " << finalPoint[3] << std::endl;
    finalPoint[0] = finalPoint[0] / finalPoint[3];
    finalPoint[1] = finalPoint[1] / finalPoint[3];
    finalPoint[2] = finalPoint[2] / finalPoint[3];
    finalPoint[3] = 0;
    std::cout << "DIVIDE BY W2: " << finalPoint[0] << ", " << finalPoint[1]  << ", " << finalPoint[2]  << ", " << finalPoint[3] << '\n' << std::endl;
    rd_write_pixel(finalPoint[0], finalPoint[1], redgreenblue);
   // }
    return RD_OK;
}

int REDirect::line_pipeline(PointH ph, bool draw)
{
    std::cout << "Pre-Current_Transform: " << ph[0] << ", " << ph[1]  << ", " << ph[2]  << ", " << ph[3] << '\n' << std::endl;
    PointH ph_trans;
    ph_trans = Matrix_PointH_Multiply(currXform, ph);
    std::cout << "Pre-Final_Transform: " << ph_trans[0] << ", " << ph_trans[1]  << ", " << ph_trans[2]  << ", " << ph_trans[3] << '\n' << std::endl;
    std::cout << "Final Transform" << std::endl;
    std::cout << final_trans[0][0] << " " << final_trans[0][1] << " " << final_trans[0][2] << " " << final_trans[0][3] << std::endl;
    std::cout << final_trans[1][0] << " " << final_trans[1][1] << " " << final_trans[1][2] << " " << final_trans[1][3] << std::endl;
    std::cout << final_trans[2][0] << " " << final_trans[2][1] << " " << final_trans[2][2] << " " << final_trans[2][3] << std::endl;
    std::cout << final_trans[3][0] << " " << final_trans[3][1] << " " << final_trans[3][2] << " " << final_trans[3][3] << '\n' << std::endl;
    PointH finalPoint;
    finalPoint = Matrix_PointH_Multiply(final_trans, ph_trans);
    std::cout << "Final Point: " << finalPoint[0] << ", " << finalPoint[1]  << ", " << finalPoint[2]  << ", " << finalPoint[3] << '\n' << std::endl;
    if (finalPoint[3] != 0)
    {
        finalPoint[0] = finalPoint[0] / finalPoint[3];
        finalPoint[1] = finalPoint[1] / finalPoint[3];
        finalPoint[2] = finalPoint[2] / finalPoint[3];
    std::cout << "DIVIDE BY W: " << finalPoint[0] << ", " << finalPoint[1]  << ", " << finalPoint[2]  << ", " << finalPoint[3] << '\n' << std::endl;
    }
    if (!draw) point_store = finalPoint;
    else if (draw)
    {
        float start[3];
        float end[3];
        start[0] = point_store[0];
        start[1] = point_store[1];
        start[2] = point_store[2];
        end[0] = finalPoint[0];
        end[1] = finalPoint[1];
        end[2] = finalPoint[2];
        cout << "DRAWING LINE" << endl;
        cout << "start: " << start[0] << " " << start[1] << " " << start[2] << endl;
        cout << "end: " << end[0] << " " << end[1] << " " << end[2] << endl; 
        line(start, end);
        point_store = finalPoint;
    } 
    return RD_OK;
}

int REDirect::rd_cube(void)
{
    PointH p1 = PointH(1, 1, -1, 1);
    PointH p2 = PointH(-1, 1, -1, 1);
    PointH p3 = PointH(-1, -1, -1, 1);
    PointH p4 = PointH(1, -1, -1, 1);
    PointH p5 = PointH(1, 1, 1, 1);
    PointH p6 = PointH(-1, 1, 1, 1);
    PointH p7 = PointH(-1, -1, 1, 1);
    PointH p8 = PointH(1, -1, 1, 1);
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
    line_pipeline(p6, false);
    line_pipeline(p2, true);
    line_pipeline(p7, false);
    line_pipeline(p3, true);
    line_pipeline(p8, false);
    line_pipeline(p4, true);

    return RD_OK;
}

int REDirect::rd_disk(float height, float radius, float theta)
{
    float newTheta;
    double PI_2 = 2 * M_PI;
    float NSTEPS = 20;
    bool draw = false;
    float x;
    float y;
    PointH p;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p.x = x;
        p.y = y;
        p.z = height;
        p.w = 1;
        line_pipeline(p, draw);
        draw = true;
    }
    return RD_OK;
}

int REDirect::rd_cylinder(float radius, float zmin, float zmax, float thetamax)
{
    float newTheta;
    double PI_2 = 2 * M_PI;
    float NSTEPS = 50;
    bool draw = false;
    float x;
    float y;
    PointH p;
    PointH p2;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p.x = x;
        p.y = y;
        p.z = zmin;
        p.w = 1;
        line_pipeline(p, draw);
        draw = true;
    }
    draw = false;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p2.x = x;
        p2.y = y;
        p2.z = zmax;
        p2.w = 1;
        line_pipeline(p2, draw);
        draw = true;
    }
    draw = false;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p.x = x;
        p.y = y;
        p.z = zmin;
        p.w = 1;
        p2.x = x;
        p2.y = y;
        p2.z = zmax;
        p2.w = 1;
        draw = false;
        line_pipeline(p, draw);
        draw = true;
        line_pipeline(p2, draw);
        draw = true;
    }
    return RD_OK;
}

int REDirect::rd_cone(float height, float radius, float thetamax)
{
    float newTheta;
    double PI_2 = 2 * M_PI;
    float NSTEPS = 50;
    bool draw = false;
    float x;
    float y;
    PointH p;
    PointH p2;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p.x = x;
        p.y = y;
        p.z = 0;
        p.w = 1;
        line_pipeline(p, draw);
        draw = true;
    }
    draw = false;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p2.x = 0;
        p2.y = 0;
        p2.z = height;
        p2.w = 1;
        line_pipeline(p2, draw);
        draw = true;
    }
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p.x = x;
        p.y = y;
        p.z = 0;
        p.w = 1;
        p2.x = 0;
        p2.y = 0;
        p2.z = height;
        p2.w = 1;
        draw = true;
        line_pipeline(p, draw);
        draw = false;
        line_pipeline(p2, draw);
        draw = true;
    }
    
    return RD_OK;
}


int REDirect::rd_sphere(float radius, float zmin, float zmax, float thetamax)
{
    float newTheta;
    double PI_2 = 2 * M_PI;
    float NSTEPS = 20;
    bool draw = false;
    float x;
    float y;
    PointH p;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta);
        p.x = x;
        p.y = y;
        p.z = zmin + zmax;
        p.w = 1;
        line_pipeline(p, draw);
        draw = true;
    }
    draw = false;
    for (int i = 0; i <= NSTEPS; i++)
    {
        newTheta = (i / NSTEPS) * PI_2;
        y = radius * sin(newTheta);
        x = radius * cos(newTheta) - sin(newTheta);
        p.x = x;
        p.y = y;
        p.z = (zmin + zmax)
        p.w = 1;
        line_pipeline(p, draw);
        draw = true;
    }
    // draw = false;
    // for (int i = 0; i <= NSTEPS; i++)
    // {
    //     newTheta = (i / NSTEPS) * PI_2;
    //     y = radius * sin(newTheta) + cos(newTheta);
    //     x = radius * cos(newTheta);
    //     p.x = x;
    //     p.y = y;
    //     p.z = (zmin + zmax);
    //     p.w = 1;
    //     line_pipeline(p, draw);
    //     draw = true;
    // }
    return RD_OK;
}