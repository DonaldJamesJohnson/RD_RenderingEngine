#include "rd_direct.h"
#include "rd_error.h"
#include "rd_display.h"

int frameNumber;
float redgreenblue[3] = {255.0, 255.0, 255.0};
float background[3] = {0.0, 0.0, 0.0};

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

int rd_color(const float color[])
{
    redgreenblue[0] = color[0];
    redgreenblue[1] = color[1];
    redgreenblue[2] = color[2];
    return RD_OK;
}

int rd_background(const float color[])
{
    return rd_set_background(color);
}

int rd_point(const float p[3])
{
    return rd_write_pixel(p[0], p[1], redgreenblue);
}

int rd_line(const float start[3], const float end[3])
{
    float xs = start[0];
    float ys = start[1];
    float zs = start[2];
    float xe = end[0];
    float ye = end[1];
    float ze = end[2];
    float y = ys;
    float p = (2 * (ye - ys)) - (xe - xs);
    float plot[3];
    for (int x = xs; x <= xe; x++)
    {
        plot[0] = x;
        plot[1] = y;
        plot[2] = zs;
        rd_point(plot);
        if (p > 0)
        {
            p += (2 * (ye - ys)) - (2 * (xe - xs));
            y++;
        }
        else p += 2 * (ye - ys);
    }
    return RD_OK;
}

int rd_circle(const float center[3], float radius)
{
    float x = center[0];
    float y = radius;
    float p = ((x + 1) * (x + 1)) + ((y - .5) * (y - .5)) - ((radius) * (radius));
    float plot[3];
    while (x <= y){
        plot[0] = x;
        plot[1] = y; 
        plot[2] = center[2];
        rd_point(plot);
        x++;
        if (p > 0)
        {
            y--;
            p += (2 * x) - (2 * y) + 1;
        }
        else 
        {
            y = y;
            p += (2 * x) + 1;
        }
    }
    return RD_OK;
}

int rd_fill(const float seed_point[3])
{
    float xs = seed_point[0];
    float xe = seed_point[1];
    float y = seed_point[2]; 
    float start[3] = {xs, y, 0};
    float end[3] = {xe, y, 0};
    rd_line(start, end);
    int new_xs;
    int new_xe;
    for (new_xs = new_xe = xs; new_xe < xe; new_xs = new_xe)
    {
        //findSpan?
    }
    return RD_OK;
}