#include "pnm_display.h"
#include "rd_error.h"
#include "rd_display.h"

int * image;

int index(int x, int y, int rgb)
{
    return image[((x * display_ySize) + y) * 3 + rgb];
}

int pnm_init_display(void)
{
    image = new int [display_xSize * display_ySize * 3];
    return RD_OK;
}

int pnm_end_display(void)
{
    delete[] image;
    return RD_OK;
}

int pnm_init_frame(int)
{
    delete[] image;
    image = new int [display_xSize * display_ySize * 3];
    return RD_OK;
}

int pnm_end_frame(void)
{
    return RD_OK;
}

int pnm_write_pixel(int x, int y, const float rgb [])
{
    image[index(x,y,0)] = rgb[0];
    image[index(x,y,1)] = rgb[1];
    image[index(x,y,2)] = rgb[2];
    return RD_OK;
}

int pnm_read_pixel(int x, int y, float rgb [])
{
    return rd_read_pixel(x, y, rgb);
}

int pnm_set_background(const float rgb [])
{
    background[0] = rgb[0];
    background[1] = rgb[1];
    background[2] = rgb[2];
    return rd_set_background(background);
}

int pnm_clear(void)
{
    return RD_OK;
}