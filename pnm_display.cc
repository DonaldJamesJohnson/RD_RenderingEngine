#include "pnm_display.h"
#include "rd_error.h"
#include "rd_display.h"

#include "algorithm"
using std::fill;

//static int image[10000][10000]; // What type of data structure is the image?

int pnm_init_display(void)
{
    return RD_OK;
}

int pnm_end_display(void)
{
    return RD_OK;
}

int pnm_init_frame(int)
{
    //Clear out image array
    return RD_OK;
}

int pnm_end_frame(void)
{
    return RD_OK;
}

int pnm_write_pixel(int x, int y, const float rgb [])
{
    return rd_write_pixel(x, y, rgb);
}

int pnm_read_pixel(int x, int y, float rgb [])
{
    return rd_read_pixel(x, y, rgb);
}

int pnm_set_background(const float rgb [])
{
    return rd_set_background(rgb);
}

int pnm_clear(void)
{
    return RD_OK;
}