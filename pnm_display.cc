#include "pnm_display.h"
#include "rd_error.h"

#include "algorithm"
using std::fill;

//static int image[10000][10000];
static int* imagex;
static int* imagey;

int pnm_init_display(void)
{
    imagex = new int[10000];
    imagey = new int[10000];
    return RD_OK;
}

int pnm_end_display(void)
{
    delete[] imagex, imagey;
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

    return RD_OK;
}

int pnm_read_pixel(int x, int y, float rgb [])
{
    return RD_OK;
}

int pnm_set_background(const float rgb [])
{
    return RD_OK;
}

int pnm_clear(void)
{
    return RD_OK;
}