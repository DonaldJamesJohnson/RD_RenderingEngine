#include "pnm_display.h"
#include "rd_error.h"
#include "rd_display.h"
#include "fstream"
#include "iostream"
#include "string"
#include "sstream"

int * image;
float background[3];

int index(int x, int y, int rgb)
{
    return ((x * display_ySize) + y) * 3 + rgb;
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

int pnm_init_frame(int fn)
{
    frameNumber = fn;
    pnm_clear();
    return RD_OK;
}

int pnm_end_frame(void)
{
    std::string oFileName;
    std::stringstream s;
    s << display_name << "_" << frameNumber <<".pnm";
    oFileName = s.str();
    std::ofstream out;
    out.open(oFileName, std::ios::binary);

    if (!out)
    {
        std::cerr << "Cannot open output file" << std::endl;
        return -1;
    }

    if (out.is_open())
    {
        out << "P6\n";
        out << display_xSize << " " << display_ySize;
        out << "\n255\n";
        for (int i = 0; i < display_ySize; i++)
        {
            for (int j = 0; j < display_xSize; j++)
            {
                out.put(image[index(j, i, 0)]);
                out.put(image[index(j, i, 1)]);
                out.put(image[index(j, i, 2)]);
            }
        }
        out.close();
    }
    return RD_OK;
}

int pnm_write_pixel(int x, int y, const float rgb [])
{
    float r = rgb[0] * 255.999;
    float g = rgb[1] * 255.999;
    float b = rgb[2] * 255.999;

    image[index(x,y,0)] = (int)r;
    image[index(x,y,1)] = (int)g;
    image[index(x,y,2)] = (int)b;
    return RD_OK;
}

int pnm_read_pixel(int x, int y, float rgb [])
{
    rgb[0] = image[index(x,y,0)];
    rgb[1] = image[index(x,y,1)];
    rgb[2] = image[index(x,y,2)];
    return RD_OK;
}

int pnm_set_background(const float rgb [])
{
    background[0] = rgb[0];
    background[1] = rgb[1];
    background[2] = rgb[2];
    return RD_OK;
}

int pnm_clear(void)
{
    float r = background[0] * 255.999;
    float g = background[1] * 255.999;
    float b = background[2] * 255.999;
    for(int i = 0; i < display_ySize; i++)
    {
        for (int j = 0; j < display_xSize; j++)
        {
            image[index(j, i, 0)] = (int)r;
            image[index(j, i, 1)] = (int)g;
            image[index(j, i, 2)] = (int)b;
        }
    }
    return RD_OK;
}