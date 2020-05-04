#include "rd_direct.h"
#include "rd_error.h"
#include "rd_display.h"
#include "iostream"
#include "Data_Structures/Vector3D.h"
#include "Data_Structures/Point.h"
#include "Data_Structures/PointH.h"
#include "Data_Structures/Matrix4D.h"
#include <stack>
#include <vector>
using std::stack;
using std::vector;
using namespace std;

int frameNumber;
float redgreenblue[3] = {1.0, 1.0, 1.0};
stack<Matrix4D> xforms;
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
double BC0[6];
int Kode0;
vector< vector<float> > zBuffer (display_xSize, vector<float>(display_ySize));
Edge * ET[480];
Edge * AET;

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
    for (int i = 0; i < display_xSize; i++)
    {
        for (int j = 0; j < display_ySize; j++)
        {
            zBuffer[i][j] = 1.0;
        }
    }
    for (int i =0;i<display_ySize;i++) ET[i] = new Edge; 		
    for (int i =0;i<display_ySize;i++) ET[i]->next = NULL; 

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
    //delete zBuffer;
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

int REDirect::point_pipeline(PointH& ph)
{
    PointH ph_trans = Matrix_PointH_Multiply(currXform, ph);
    PointH ph_cam = Matrix_PointH_Multiply(world_to_cam, ph_trans);
    PointH ph_clip = Matrix_PointH_Multiply(cam_to_clip, ph_cam);
    PointH ph_device = Matrix_PointH_Multiply(clip_to_device, ph_clip);    
    ph_device[0] = ph_device[0] / ph_device[3];
    ph_device[1] = ph_device[1] / ph_device[3];
    ph_device[2] = ph_device[2] / ph_device[3];
    ph_device[3] = 0;
    zBuff_write_pixel(ph_device[0], ph_device[1], ph_device[2]);
    return RD_OK;
}

int REDirect::line_pipeline(PointH ph, bool draw)
{
    PointH ph_trans = Matrix_PointH_Multiply(currXform, ph);
    PointH ph_cam = Matrix_PointH_Multiply(world_to_cam, ph_trans);
    PointH ph_clip = Matrix_PointH_Multiply(cam_to_clip, ph_cam);
    Clip(ph_clip, draw);
    return RD_OK;
}

int REDirect::poly_pipeline(attr_point p, int end_flag)
{
    PointH geom, norm;
    PointH dev;

    const int MAX_VERTEX_LIST_SIZE = 50;
    static attr_point  vertex_list[MAX_VERTEX_LIST_SIZE];
    static attr_point  clipped_list[MAX_VERTEX_LIST_SIZE];
    static int n_vertex = 0;

    int i;

    // Run geometry through current transform
    geom[0] = p.coord[0];
    geom[1] = p.coord[1];
    geom[2] = p.coord[2];
    geom[3] = p.coord[3];

    geom = Matrix_PointH_Multiply(currXform, geom);

    // Run through world to clip
    geom = Matrix_PointH_Multiply(world_to_cam, geom);
    geom = Matrix_PointH_Multiply(cam_to_clip, geom);

    p.coord[0] = geom[0];
    p.coord[1] = geom[1];
    p.coord[2] = geom[2];
    p.coord[3] = geom[3];

    // Store in vertex list
    if( n_vertex == MAX_VERTEX_LIST_SIZE)
    return -1;  // Overflow

    vertex_list[n_vertex] = p;
    n_vertex++;

    if(end_flag == false)  // Move along to the next vertex
    return 0;

  if((n_vertex = poly_clip(n_vertex, vertex_list, clipped_list)))
    {
      // There's something left! --- Let's draw it

      // Pre process vertex list
      for(i = 0; i < n_vertex; i++)
	{
	  // Convert geometry to device coordinates
	  dev[0] = clipped_list[i].coord[0];
	  dev[1] = clipped_list[i].coord[1];
	  dev[2] = clipped_list[i].coord[2];
	  dev[3] = clipped_list[i].coord[3];
	  dev = Matrix_PointH_Multiply(clip_to_device, dev);
	  
	  clipped_list[i].coord[0] = dev[0];
	  clipped_list[i].coord[1] = dev[1];
	  // clipped_list[i].coord[2] = dev[2];  // Superfluous
	  // clipped_list[i].coord[3] = dev[3];

	  // Divide geometry by w.
	  clipped_list[i].coord[0] /= clipped_list[i].coord[3];
	  clipped_list[i].coord[1] /= clipped_list[i].coord[3];
	  clipped_list[i].coord[2] /= clipped_list[i].coord[3];

	}

      scan_convert(clipped_list, n_vertex);
    }

  // Reset structures for next polygon

  n_vertex = 0;
}

int REDirect::DDA_Line(const float start[3], const float end[3])
{
    int x0 = (int)start[0];
    int y0 = (int)start[1];
    float z0 = start[2];
    int x1 = (int)end[0];
    int y1 = (int)end[1];
    float z1 = end[2];
    float x = x0;
    float y = y0;
    float z = z0;
    int NSTEPS;
    float dx = x1 - x0;
    float dy = y1 - y0;
    float dz = z1 - z0;

    if (abs(dy) > abs(dx)) NSTEPS = abs(dy);
    else NSTEPS = abs(dx);
    dx /= NSTEPS;
    dy /= NSTEPS;
    dz /= NSTEPS;
    for (int i = 0; i < NSTEPS; i++)
    {
        zBuff_write_pixel(x, y, z);
        x += dx;
        y += dy;
        z += dz;
    }

}

int REDirect::zBuff_write_pixel(float x, float y, float z)
{
    if (z < zBuffer[x][y])
    {
        rd_write_pixel(x, y, redgreenblue);
        zBuffer[x][y] = z;
    }
    return RD_OK;
}

int REDirect::rd_cube(void)
{
    PointH p1 = PointH(-1, -1, -1, 1);
    attr_point ap1;
    ap1.coord[0] = p1[0];
    ap1.coord[1] = p1[1];
    ap1.coord[2] = p1[2];
    ap1.coord[3] = p1[3];
    PointH p2 = PointH(1, -1, -1, 1);
    attr_point ap2;
    ap2.coord[0] = p2[0];
    ap2.coord[1] = p2[1];
    ap2.coord[2] = p2[2];
    ap2.coord[3] = p2[3];
    PointH p3 = PointH(1, 1, -1, 1);
    attr_point ap3;
    ap3.coord[0] = p3[0];
    ap3.coord[1] = p3[1];
    ap3.coord[2] = p3[2];
    ap3.coord[3] = p3[3];
    PointH p4 = PointH(-1, 1, -1, 1);
    attr_point ap4;
    ap4.coord[0] = p4[0];
    ap4.coord[1] = p4[1];
    ap4.coord[2] = p4[2];
    ap4.coord[3] = p4[3];
    // PointH p5 = PointH(1, 1, 1, 1);
    // PointH p6 = PointH(-1, 1, 1, 1);
    // PointH p7 = PointH(-1, -1, 1, 1);
    // PointH p8 = PointH(1, -1, 1, 1);
    poly_pipeline(ap1, false);
    poly_pipeline(ap2, false);
    poly_pipeline(ap3, false);
    poly_pipeline(ap4, true);
    // line_pipeline(p1, true);
    // line_pipeline(p5, true);
    // line_pipeline(p6, true);
    // line_pipeline(p7, true);
    // line_pipeline(p8, true);
    // line_pipeline(p5, true);
    // line_pipeline(p6, false);
    // line_pipeline(p2, true);
    // line_pipeline(p7, false);
    // line_pipeline(p3, true);
    // line_pipeline(p8, false);
    // line_pipeline(p4, true);

    return RD_OK;
}

int REDirect::rd_disk(float height, float radius, float theta)
{
    float newTheta;
    double PI_2 = 2 * M_PI;
    float NSTEPS = theta;
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
    float NSTEPS = thetamax / 10;
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
    double theta;
    double phi;
    double x, y, z;
    double radians = (M_PI/180);
    int NSTEPS = 20;
    int NSTEPSX = 10;
    PointH p;
    
    for (int j = 0; j < NSTEPS; j++)
    {
        for (int i = 0; i < NSTEPSX; i++)
        {
            theta = double(((double(i)/NSTEPSX))*(360));
            phi = double(((double(j)/NSTEPS)*180)-90);
            x = radius * cos(theta * radians) * cos(phi * radians);
            y = radius * sin(theta * radians) * cos(phi * radians);
            z = radius * sin(phi * radians);
            p[0] = x; 
            p[1] = y; 
            p[2] = z; 
            p[3] = 1;
            line_pipeline(p, false);
            theta = double(((double(i+1)/NSTEPSX))*(360));
            x = radius * cos(theta * radians) * cos(phi * radians);
            y = radius * sin(theta * radians) * cos(phi * radians);
            z = radius * sin(phi * radians);
            p[0] = x; 
            p[1] = y; 
            p[2] = z; 
            p[3] = 1;
            line_pipeline(p, true);
            phi = double(((double(j+1)/NSTEPS)*180)-90);
            x = radius * cos(theta * radians) * cos(phi * radians);
            y = radius * sin(theta * radians) * cos(phi * radians);
            z = radius * sin(phi * radians);
            p[0] = x; 
            p[1] = y; 
            p[2] = z; 
            p[3] = 1;
            line_pipeline(p, true);
            theta = double(((double(i)/NSTEPSX))*(360));
            x = radius * cos(theta * radians) * cos(phi * radians);
            y = radius * sin(theta * radians) * cos(phi * radians);
            z = radius * sin(phi * radians);
            p[0] = x; 
            p[1] = y; 
            p[2] = z; 
            p[3] = 1;
            line_pipeline(p, true);
            phi = double(((double(j)/NSTEPS)*180)-90);
            x = radius * cos(theta * radians) * cos(phi * radians);
            y = radius * sin(theta * radians) * cos(phi * radians);
            z = radius * sin(phi * radians);
            p[0] = x; 
            p[1] = y; 
            p[2] = z; 
            p[3] = 1;
            line_pipeline(p, true);
        }
    }
    return RD_OK;
}

int REDirect::rd_pointset(const string & vertex_type,int nvertex, const float * vertex) 
{  	
    float p[3]; 	 	
    for(int i = 0; i < nvertex; i++) 	
    { 	    
        p[0]=vertex[i*3+0]; 		
        p[1]=vertex[i*3+1]; 		
        p[2]=vertex[i*3+2]; 		
        rd_point(p); 	
    } 	
return RD_OK; 
}
int REDirect::rd_polyset(const string & vertex_type, int nvertex, const float * vertex, int nface, const int * face)
{
    float p[3];
    PointH ph;
    bool draw = false;
    int size;
    bool iterate = true;
    for (int i = 0; i < nface; i++)
    {
        int j = 3;
        int inc = 0;
        int val;
        while(iterate)
        {
            while (val != -1)
            {
                val = face[i * j + inc];
                inc++;
                size = inc;
            }
            iterate = false;
        }
    }
    int v[size];
    std::cout << "size: " << size << endl;		
    for(int i = 0; i < nface; i++) 	
    {
        for (int k = 0; k < size-1; k++) 
        {
            v[k] = face[i*(size)+k];  
        }
        if ((face[i*size + (size-1)]) == -1) 
        {
            v[(size-1)] = v[0];
        }
        for (int j = 0; j < size; j++)
        {
            p[0] = vertex[v[j]*3+0];
            p[1] = vertex[v[j]*3+1];
            p[2] = vertex[v[j]*3+2];
            ph[0] = p[0];
            ph[1] = p[1];
            ph[2] = p[2];
            ph[3] = 1;
            line_pipeline(ph, draw);
            draw = true;
        }
        draw = false;
    } 
    return RD_OK;
}

int REDirect::Clip(PointH p1, bool draw)
{
    int Kode1 = 0;
    double BC1[6];
    int mask = 1;

    // To match Blinn's notation...
    PointH p0 = point_store;

    // Calculate BC1
    BC1[0] = p1[0];
    BC1[1] = p1[3] - p1[0];
    BC1[2] = p1[1];
    BC1[3] = p1[3] - p1[1];
    BC1[4] = p1[2];
    BC1[5] = p1[3] - p1[2];
    
    // Calculate Kode1
    for (int i = 0; i < 6; i++, mask<<=1) if (BC1[i] < 0) Kode1 |= mask;
    
    if (draw)
    {
        if ((Kode0 & Kode1) == 0) // Else trivial reject
        {
            if ((Kode0 | Kode1) == 0) // Trivial accept
            {
                PointH p0_device = Matrix_PointH_Multiply(clip_to_device, p0);    
                p0_device[0] = p0_device[0] / p0_device[3];
                p0_device[1] = p0_device[1] / p0_device[3];
                p0_device[2] = p0_device[2] / p0_device[3];
                p0_device[3] = 1;
                PointH p1_device = Matrix_PointH_Multiply(clip_to_device, p1);    
                p1_device[0] = p1_device[0] / p1_device[3];
                p1_device[1] = p1_device[1] / p1_device[3];
                p1_device[2] = p1_device[2] / p1_device[3];
                p1_device[3] = 1;

                float start[3];
                float end[3];
                start[0] = p0_device[0];
                start[1] = p0_device[1];
                start[2] = p0_device[2];
                end[0] = p1_device[0];
                end[1] = p1_device[1];
                end[2] = p1_device[2];
                DDA_Line(start, end);
            }
            else 
            {
                int Klip = (Kode0 | Kode1);
                double amin = 0.0;
                double amax = 1.0;
                mask = 1;
                bool nonTrivReject = false;
                for (int i = 0; i < 6; i++, mask<<=1)
                {
                    if ((Klip & mask) != 0)
                    {
                        double a = (BC0[i] / (BC0[i] - BC1[i]));                    
                        if ((Kode0 & mask) != 0) amin = max(amin, a);
                        else amax = min(amax, a);
                        if (amax < amin) 
                        {
                            nonTrivReject = true;
                            break;
                        }
                    }                   
                }
                if (!nonTrivReject)
                {
                    PointH p_draw;
                    for (int i = 0; i < 4; i++) p_draw[i] = p0[i] + (amin * (p0[i] - p1[i]));
                    Clip(p_draw, false);

                    PointH p_draw2;
                    for (int i = 0; i < 4; i++) p_draw2[i] = p0[i] + (amax * (p0[i] - p1[i]));                        
                    Clip(p_draw2, true);
                }                
            }
        }
    }

    point_store = p1;
    for (int i = 0; i < 6; i++) BC0[i] = BC1[i];
    Kode0 = Kode1;

    return RD_OK;
}

void REDirect::scan_convert(attr_point points[], int count)
{
    if(! buildEdgeList(points, count)) return;   // No edges cross a scanline

    buildEdgeList(points, count);
    delete[] AET;
    AET = new Edge;
    AET->next = NULL;

    for (int scan = 0; scan < display_ySize; scan++)
    {
        //Take the edges starting on this scanline from the edge table
        //and add them to the active edge table (AET).
        addActiveList(scan, AET);

        if (AET->next)
           {
               //fill between the edge pairs in the AET
               fillBetweenEdges(scan, AET);
               //update the AET
               updateAET(scan, AET);
               //resort the AET
               resortAET(AET);
           }
        }
}

bool REDirect::buildEdgeList(attr_point attr_points[], int count)
{
    bool scanline_crossed = false;
    int v1 = count - 1;  // The last vertex in the polygon
    int v2 = 0;
    for (v2; v2 < count; v2++)
     { 
        if(attr_points[v1].coord[1] != attr_points[v2].coord[1]){
           scanline_crossed = true;
            
           if(attr_points[v1].coord[1]  < attr_points[v2].coord[1])
              makeEdgeRec(attr_points[v1], attr_points[v2]);
           else
              makeEdgeRec(attr_points[v2], attr_points[v1]);
        }
        v1 = v2; // Move to next edge
     }
    return scanline_crossed;
}

int REDirect::makeEdgeRec(attr_point upper, attr_point lower)
{
    float dy = upper.coord[1] - lower.coord[1];

    Edge * e = new Edge;

    // Calculate the edge value increments between scan lines
    e->inc.coord[0] = ((upper.coord[0] - lower.coord[0]) / dy);
    e->inc.coord[1] = ((upper.coord[1] - lower.coord[1]) / dy);
    e->inc.coord[2] = ((upper.coord[2] - lower.coord[2]) / dy);

    // Edge starts on scanline ceil(lower.y)
    float factor =  ceil(lower.coord[1]) - lower.coord[1];  // Gives the fractional position of the first scanline crossing

    // Calculate the starting values for the edge
    e->p.coord[0] = lower.coord[0] + factor * (e->inc.coord[0]);
    e->p.coord[1] = lower.coord[1] + factor * (e->inc.coord[1]);
    e->p.coord[2] = lower.coord[2] + factor * (e->inc.coord[2]);

    // Find the last scanline for the edge
    e->yLast = ceil(upper.coord[1]) - 1;

    int index = ceil(lower.coord[1]);
    index = abs(index);
    insertEdge(ET[index], e);
    return RD_OK;
}

int REDirect::addActiveList(int scan, Edge * AET)
{
    Edge *p, *q;

    p = ET[scan]->next; // Get the edges starting on this scan line
    while(p)
      {
        q = p->next;  // Hold the rest of the list
        insertEdge(AET, p);
        p = q;
      }

    // Keep edge table clean -- edges have been transfered
    ET[scan]->next = 0;
    return RD_OK;
}

int REDirect::insertEdge(Edge * list, Edge * e)
{
    Edge *p;
    Edge *q = list;

    // p leads
    p = q->next;
    int i = 0;
    while(p != 0 && (e->p.coord[0] > p->p.coord[0]))
    {
        cout << "p: " << p->p.coord[0] << endl;
        // Step to the next edge
        q = p;
        p = p->next;
        i++;
    }

    // Link the edge into the list after q
    e->next = q->next;
    q->next = e;
    return RD_OK;
}

int REDirect::updateAET(int scanline, Edge * AET)
{
    Edge *q = AET, *p = AET->next;
    // p leads

    while(p)
    {
        if(scanline == p->yLast)  // This is the last scanline for this edge
        {
            p = p->next;    // Move p along
            deleteAfter(q);  // get rid of old node
        }
        else // Update the attribute values
        {
            for (int i = 0; i < 16; i++) p->p.coord[i] += p->inc.coord[i];            
            q = p;
            p = p->next;
        }
    }
    return RD_OK;
}

int REDirect::deleteAfter(Edge * q)
{
    Edge *p = q->next;

    q->next = p->next;
    delete p;
    return RD_OK;
}

int REDirect::resortAET(Edge * AET)
{
    Edge *q, *p = AET->next;

    AET->next = NULL;
    while(p)
    {
        q = p->next;
        insertEdge(AET, p);
        p = q;
    }
    return RD_OK;
}

int REDirect::fillBetweenEdges(int scan, Edge * AET)
{
    Edge * p1; 
    Edge * p2;
  
    p1 = AET->next;

    while (p1)
    {
        p2 = p1->next;  // Get the pair of edges from the AET
        //cout << "p1.x: " << p1->p.coord[0] << endl;
        //cout << "p2.x: " << p2->p.coord[0] << endl;
        if(p1->p.coord[0] != p2->p.coord[0])
        {
            // Calculate the attribute increments along the scanline
            float dx = p2->p.coord[0] - p1->p.coord[0];
            p1->inc.coord[0] = (p2->p.coord[0] - p1->p.coord[0]) / dx;
            p1->inc.coord[1] = (p2->p.coord[1] - p1->p.coord[1]) / dx;
            p1->inc.coord[2] = (p2->p.coord[2] - p1->p.coord[2]) / dx;

                // Calculate the starting values for the edge
            float factor = ceil(p1->p.coord[0]) - p1->p.coord[0]; // Gives the fractional position
                                        // of the first pixel crossing

            attr_point value;
            value.coord[0] = p1->p.coord[0] + factor * p1->inc.coord[0];
            value.coord[1] = p1->p.coord[1] + factor * p1->inc.coord[1];
            value.coord[2] = p1->p.coord[2] + factor * p1->inc.coord[1];


            float endx = ceil(p2->p.coord[0]);
            cout << "while" << endl;
            while(value.coord[0] < endx)
            {
                //Calculate the color for the pixel and plot it.
                //x and z come from the current values, y is the current scanline
               PointH plot;
               plot[0] = value.coord[0]; 		  
               plot[1] = scan; 		  
               plot[2] = value.coord[0];
               cout << "plot: " << plot[0] << " " << plot[1] << " " << plot[2] << endl;           
               point_pipeline(plot);               
               // Increment the values 		  
               value.coord[0] = value.coord[0] + p1->inc.coord[0]; 		  
               value.coord[1] = value.coord[1] + p1->inc.coord[1]; 		  
               value.coord[2] = value.coord[2] + p1->inc.coord[2];
            }
        }
        p1 = p2->next;
    }
    return RD_OK;
}

int REDirect::poly_clip(int points, attr_point in[], attr_point out[])
{
    attr_point first[6] = {NULL};
    attr_point last[6] = {NULL};
    bool flags[6] = {false};
    int count = 0;

    for(int i =0;i< points;i++) count += clipPoint(in[i], out, 0, first, last, flags, count);

    return count;  
}

int REDirect::clipPoint(attr_point p, attr_point out[], int b, attr_point first[], attr_point last[], bool flags[], int count)
{
     if (!flags[b])
        {
            first->coord[b] = p.coord[b];
            last->coord[b] = p.coord[b];
            flags[b] = true;
        }
        else
        {
            if (cross(p, last[b], b))
            {
                attr_point ipt = intersect(p, last[b], b);
                if (b < 6) count += clipPoint(ipt, out, b+1, first, last, flags, count);
                else out[count++] = ipt; 
            }
        }
        last[b] = p;

        if (inside(p, b))
        {
            if (b < 6) count += clipPoint(p, out, b+1, first, last, flags, count);
            else out[count++] = p; 
        }

    return count;
}

int REDirect::clipLastPoint(attr_point p, attr_point out[], int b, attr_point first[], attr_point last[], bool flags[], int count)
{
    for (int i = 0; i < 6; i++)
    {
        if (first[b].coord[0] != NULL & first[b].coord[1] != NULL && first[b].coord[2] != NULL && cross(first[b], last[b], b))
        {
            attr_point ipt = intersect(last[b], first[b], b);
            if (b < 6) count += clipPoint(ipt, out, b+1, first, last, flags, count);
            else out[count++] = ipt;
        }
    }
    return count;
}

attr_point intersect(attr_point p1, attr_point p2, int b)
{
    float a;
    double BC0[6];
    attr_point p;
    BC0[0] = p1.coord[0];
    BC0[1] = p1.coord[3] - p1.coord[0];
    BC0[2] = p1.coord[1];
    BC0[3] = p1.coord[3] - p1.coord[1];
    BC0[4] = p1.coord[2];
    BC0[5] = p1.coord[3] - p1.coord[2];
    double BC1[6];
    BC1[0] = p2.coord[0];
    BC1[1] = p2.coord[3] - p2.coord[0];
    BC1[2] = p2.coord[1];
    BC1[3] = p2.coord[3] - p2.coord[1];
    BC1[4] = p2.coord[2];
    BC1[5] = p2.coord[3] - p2.coord[2];
    a = BC0[b] / (BC0[b]- BC1[b]);
    for (int i = 0; i < 16; i++) p.coord[i] = p1.coord[i] + a * (p2.coord[i] - p1.coord[i]);
    return p;
}

bool inside(attr_point p, int b)
{
    BC0[0] = p.coord[0];
    BC0[1] = p.coord[3] - p.coord[0];
    BC0[2] = p.coord[1];
    BC0[3] = p.coord[3] - p.coord[1];
    BC0[4] = p.coord[2];
    BC0[5] = p.coord[3] - p.coord[2];
    for (int i = 0; i < 6; i++)
    {
        if (b == i)
        {
            if (b % 2 == 0)
            {
                if (BC0[i] > 0) return true;
                else return false;
            }
            else 
            {
                if (BC0[i] < 1) return true;
                else return false;
            }
        }
    }
}

bool cross(attr_point p1, attr_point p2, int b)
{
    bool cross = false;
    if (p1.coord[0] < 0 && p2.coord[0] > 0 || p1.coord[0] > 0 && p2.coord[0] < 0) cross = true;
    if (p1.coord[3] - p1.coord[0] < 0 && p2.coord[3] - p2.coord[0] > 0 || p1.coord[3] - p1.coord[0] > 0 && p2.coord[3] - p2.coord[0] < 0) cross = true;
    if (p1.coord[1] < 0 && p2.coord[1] > 0 || p1.coord[1] > 0 && p2.coord[1] < 0) cross = true;
    if (p1.coord[3] - p1.coord[1] < 0 && p2.coord[3] - p2.coord[1] > 0 || p1.coord[3] - p1.coord[1] > 0 && p2.coord[3] - p2.coord[1] < 0) cross = true;
    if (p1.coord[2] < 0 && p2.coord[2] > 0 || p1.coord[2] > 0 && p2.coord[2] < 0) cross = true;
    if (p1.coord[3] - p1.coord[2] < 0 && p2.coord[3] - p2.coord[2] > 0 || p1.coord[3] - p1.coord[2] > 0 && p2.coord[3] - p2.coord[2] < 0) cross = true;
    return cross;
}