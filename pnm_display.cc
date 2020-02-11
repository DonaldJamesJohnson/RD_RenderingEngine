#include "pnm_display.h"
#include "rd_error.h"

static int* image;

int pnm_init_display(void)
{
    image = new int[100000];
    return RD_OK;
}

int pnm_end_display(void)
{
    delete[] image;
    return RD_OK;
}

int pnm_init_frame(int)
{
    image = {0};
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