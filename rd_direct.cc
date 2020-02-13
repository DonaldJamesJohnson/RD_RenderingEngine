#include "rd_direct.h"
#include "rd_error.h"
#include "rd_display.h"
#include "iostream"

int frameNumber;
float redgreenblue[3] = {1.0, 1.0, 1.0};
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
    std::cout << "--------------------------------" << std::endl;
    std::cout << "start[0]: " << start[0] << " | end[0]: " << end[0]<< std::endl;
    std::cout << "start[1]: " << start[1] << " | end[1]: " << end[1] << std::endl;
    line(start, end);
    return RD_OK;
}

int REDirect::rd_circle(const float center[3], float radius)
{
    float x = center[0];
    float y = radius;
    float p = ((x) * (x)) + ((y - .5) * (y - .5)) - ((radius) * (radius));
    float plot[3];
    while (x <= y){
        plot[0] = x;
        plot[1] = y; 
        plot[2] = center[2];
        rd_write_pixel(plot[0], plot[1], redgreenblue);
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

int REDirect::rd_fill(const float seed_point[3]) //xyz 
{
    // use find span to determine xs and xe
    // look at seed point color and set seed_color to that
    int seed_color;
    float xs = seed_point[0];
    float xe = seed_point[1];
    float y = seed_point[2]; 
    float start[3] = {xs, y, 0};
    float end[3] = {xe, y, 0};
    REDirect::rd_line(start, end);
    int new_xs;
    int new_xe;
    for (new_xs = new_xe = xs; new_xe < xe; new_xs = new_xe)
    {
        
    }
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
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "dx: " << dx << " | dy: " << dy << std::endl;
    std::cout << "p = " << p << std::endl;

    if (start[0] < end[0] && start[1] <= end[1] && dx > dy)        // Case 1
    {
        std::cout << "Case 1" << std::endl;
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
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
            else 
            {
                p += (2 * dy);
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
        }
    }
    else if (start[0] < end[0] && start[1] < end[1] && dx <= dy)    // Case 2
    {
        std::cout << "Case 2" << std::endl;
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
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
            else 
            {
                p += (2 * dx);
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
        }
    }
    else if (start[0] >= end[0] && start[1] < end[1] && dx < dy)   // Case 3
    {
        std::cout << "Case 3" << std::endl;
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
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
            else 
            {
                p += (2 * dx);
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
        }
    }
    else if (start[0] > end[0] && start[1] < end[1] && dx >= dy)    // Case 4
    {
        std::cout << "Case 4" << std::endl;
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
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
            else 
            {
                p += (2 * dy);
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
        }
    }
    else if (start[0] > end[0] && start[1] >= end[1] && dx > dy)   // Case 5
    {
        std::cout << "Case 5" << std::endl;
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
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
            else 
            {
                p += (2 * dy);
                std::cout << "x: " << x;
                std::cout << "| y: " << y;
                std::cout << "| p: " << p << std::endl;
            }
        }
    }
    else if (start[0] > end[0] && start[1] > end[1] && dx <= dy)    // Case 6
    {

    }
    else if (start[0] <= end[0] && start[1] > end[1] && dx < dy)   // Case 7
    {
        std::cout << "Case 7" << std::endl;
    }
    else if (start[0] < end[0] && start[1] > end[1] && dx >= dy)    // Case 8
    {

    }
}