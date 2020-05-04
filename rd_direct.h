#ifndef RD_ENGINE_DIRECT_H
#define RD_ENGINE_DIRECT_H

#include "rd_enginebase.h"
#include "pnm_display.h"
#include "Data_Structures/PointH.h"
#include "Data_Structures/Point.h"
#include <string>
#include <list> 
#define ATTR_CONSTANT  4
#define ATTR_R         5
#define ATTR_G         6
#define ATTR_B         7
#define ATTR_NX        8
#define ATTR_NY        9
#define ATTR_NZ       10
#define ATTR_S        11
#define ATTR_T        12
#define ATTR_WORLD_X  13
#define ATTR_WORLD_Y  14
#define ATTR_WORLD_Z  15
#define ATTR_SIZE     16

typedef struct {
  float coord[ATTR_SIZE];
} attr_point;

struct Edge
{
   int  yLast;     // Final scan line of edge
   attr_point  p;   // The values of the edge on this scan line
   attr_point  inc; // The incremental changes for the values from scanline to scanline
   Edge * next;     // A pointer to link the edges together in edge tables.
};

using std::string;
using std::list;

// This is a rendering engine that renders directly to the image buffer as
// primitives come in.  A depth buffer is obviously needed.  Transparency is
// not properly handled.

class REDirect: public RenderEngine
{
public:
   /**********************   General functions  *******************************/

  int rd_display(const string & name, const string & type, const string & mode);

  int rd_format(int xresolution, int yresolution);

  int rd_world_begin(void);

  int rd_world_end(void);

  int rd_frame_begin(int frame_no);

  int rd_frame_end(void);

  int rd_render_init(void);

  int rd_render_cleanup(void);

  int rd_background(const float color[]);

  int rd_color(const float color[]);

  int rd_point(const float p[3]);

  int rd_line(const float start[3], const float end[3]);

  void line(const float start[3], const float end[3]);

  int rd_circle(const float center[3], float radius);

  void circle(const float center[3], float radius);

  int rd_fill(const float seed_point[3]);
  
  void fill(int x, int y, float seed_color[3], float new_color[3]);

  int rd_translate(const float offset[3]);
  int rd_scale(const float scale_factor[3]);
  int rd_rotate_xy(float angle);
  int rd_rotate_yz(float angle);
  int rd_rotate_zx(float angle);
  
  int rd_xform_push(void);
  int rd_xform_pop(void);

  int rd_pointset(const string & vertex_type,
			  int nvertex, const float * vertex);
  int rd_polyset(const string & vertex_type, 
			 int nvertex, const float * vertex,
			 int nface,   const int * face);

  int rd_camera_eye(const float eyepoint[3]);
  int rd_camera_at(const float atpoint[3]);
  int rd_camera_up(const float up[3]);
  int rd_camera_fov(float fov);
  int rd_clipping(float znear, float zfar);

  int rd_cone(float height, float radius, float thetamax);
  int rd_cube(void);
  int rd_cylinder(float radius, float zmin, float zmax, float thetamax);
  int rd_disk(float height, float radius, float theta);
  int rd_sphere(float radius, float zmin, float zmax, float thetamax);

 int point_pipeline(PointH& ph);

 int line_pipeline(PointH ph, bool draw);
 int poly_pipeline(attr_point p, int end_flag);

 int Clip(PointH ph_clip, bool draw);

 int zBuff_write_pixel(float x, float y, float z);
 int DDA_Line(const float start[3], const float end[3]);

 void scan_convert(attr_point attr_points[], int count);
 bool buildEdgeList(attr_point attr_points[], int count);
 int makeEdgeRec(attr_point upper, attr_point lower);
 int addActiveList(int scanline, Edge * AET);
 int insertEdge(Edge * list, Edge * e);
 int updateAET(int scanline, Edge * AET);
 int deleteAfter(Edge * q);
 int resortAET(Edge * AET);
 int fillBetweenEdges(int scanline, Edge * AET);
 int poly_clip(int points, attr_point in[], attr_point out[]);
 int clipPoint(attr_point p, attr_point out[], int b, attr_point first[], attr_point last[], bool flags[], int count);
 int clipLastPoint(attr_point p, attr_point out[], int b, attr_point first[], attr_point last[], bool flags[], int count);
};

#endif /* RD_ENGINE_DIRECT_H */
