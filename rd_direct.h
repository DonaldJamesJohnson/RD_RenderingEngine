#ifndef RD_ENGINE_DIRECT_H
#define RD_ENGINE_DIRECT_H

#include "rd_enginebase.h"
#include "pnm_display.h"

#include <string>
using std::string;

// This is a rendering engine that renders directly to the image buffer as
// primitives come in.  A depth buffer is obviously needed.  Transparency is
// not properly handled.

class REDirect: public RenderEngine
{
 public:
   /**********************   General functions  *******************************/

    int rd_display(const string & name, const string & type, const string & mode)
    {

    }

   int rd_format(int xresolution, int yresolution)
   {

   }

   int rd_world_begin(void)
   {

   }
   int rd_world_end(void)
   {

   }

   int rd_frame_begin(int frame_no)
   {

   }
   int rd_frame_end(void)
   {

   }

   int rd_render_init(void)
   {

   }
   int rd_render_cleanup(void)
   {
       
   }

  
  int pnm_init_display(void);

  int pnm_end_display(void);

  int pnm_init_frame(int);

  int pnm_end_frame(void);

  int pnm_write_pixel(int x, int y, const float rgb []);

  int pnm_read_pixel(int x, int y, float rgb []);

  int pnm_set_background(const float rgb []);

  int pnm_clear(void);

};

#endif /* RD_ENGINE_DIRECT_H */
