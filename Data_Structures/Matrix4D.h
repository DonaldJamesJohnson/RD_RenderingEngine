#ifndef __MATRIX4D_H__
#define __MATRIX4D_H__

#include "Vector3D.h"
#include "Point.h"
#include "PointH.h"
#include <iostream>
#include <math.h>
using namespace std;

struct Matrix4D
{
private:
    double n[4][4];

public:
/**************************** Constructors *****************************/
    Matrix4D() = default;

    Matrix4D(double n00, double n01, double n02, double n03,
             double n10, double n11, double n12, double n13,
             double n20, double n21, double n22, double n23,
             double n30, double n31, double n32, double n33)
    {
        n[0][0] = n00; n[0][1] = n01; n[0][2] = n02; n[0][3] = n03;
        n[1][0] = n10; n[1][1] = n11; n[1][2] = n12; n[1][3] = n13;
        n[2][0] = n20; n[2][1] = n21; n[2][2] = n22; n[2][3] = n23;
        n[3][0] = n30; n[3][1] = n31; n[3][2] = n32; n[3][3] = n33;
    }

    Matrix4D(const Vector3D& a, const Vector3D& b, const Vector3D& c)
    {
        n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[0][3] = 1;
        n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z; n[1][3] = 1;
        n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z; n[2][3] = 1;
        n[3][0] = 0;   n[3][1] = 0;   n[3][2] = 0;   n[3][3] = 1;
    }

    /**************************** Operators *****************************/

    double& operator ()(int i, int j)
    {
        return (n[i][j]);
    }

    const double& operator ()(int i, int j) const
    {
        return (n[i][j]);
    }

    Vector3D& operator [](int j)
    {
        return (*reinterpret_cast<Vector3D *>(n[j]));
    }

    const Vector3D& operator [](int j) const
    {
        return (*reinterpret_cast<const Vector3D *>(n[j]));
    }

    /**************************** Transformations *****************************/

    Matrix4D identity()
    {

        return (Matrix4D(1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         0.0, 0.0, 0.0, 1.0));
    }

    Matrix4D SetTranslation(const Point& p)
    {
        return (Matrix4D(1.0, 0.0, 0.0, p[0],
                         0.0, 1.0, 0.0, p[1],
                         0.0, 0.0, 1.0, p[2],
                         0.0, 0.0, 0.0, 1.0));
    }

    Matrix4D MakeRotationX(double t)
    {
        double c = cos(t);
        double s = sin(t);
        return (Matrix4D(1.0, 0.0, 0.0, 0.0,
                         0.0,   c,  -s, 0.0,
                         0.0,   s,   c, 0.0,
                         0.0, 0.0, 0.0, 1.0));
    }

    Matrix4D MakeRotationY(double t)
    {
        double c = cos(t);
        double s = sin(t);
        return (Matrix4D(  c, 0.0,   s, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                          -s, 0.0,   c, 0.0,
                         0.0, 0.0, 0.0, 1.0));
    }

    Matrix4D MakeRotationZ(double t)
    {
        double c = cos(t);
        double s = sin(t);
        return (Matrix4D(  c,  -s, 0.0, 0.0,
                           s,   c, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         0.0, 0.0, 0.0, 1.0));
    }

    Matrix4D MakeScale(double sx, double sy, double sz)
    {
        return (Matrix4D(sx, 0.0, 0.0, 0.0,
                         0.0, sy, 0.0, 0.0,
                         0.0, 0.0, sz, 0.0,
                         0.0, 0.0, 0.0, 1.0));
    }

    /**************************** Graphics Pipeline *****************************/

    Matrix4D world_to_camera(const Point& eye, const Point& at, const Vector3D& up)
    {
        Vector3D At = Point_Point_Subtract(at, eye).to_vector();
        At = At.Normalize();

        Vector3D V = At.Cross(up);
        V = V.Normalize();
       
        Vector3D U = V.Cross(At);
        U = U.Normalize();

        return (Matrix4D( V.x,  V.y,  V.z, -eye.x,
                          U.x,  U.y,  U.z, -eye.y,
                         At.x, At.y, At.z, -eye.z,
                          0.0,  0.0,  0.0, 1.0));
    }

    Matrix4D camera_to_clip(double fov, double near, double far, double width, double height)
    {
        double tan_theta = tan((fov/2) * M_PI / 180.0);
        double aspect = width/height;

        return (Matrix4D((0.5/(aspect * tan_theta)), 0.0 , 0.5, 0.0,
                         0.0, (0.5/tan_theta), 0.5, 0.0,
                         0.0, 0.0, (far / (far-near)), (-(far * near) / (far - near)),
                         0.0, 0.0, 0.0, 1.0));
    }

    Matrix4D clip_to_device(int width, int height)
    {
        double w = width * 1.0;
        double h = height * 1.0;
        double nh = h * -1.0;

        return (Matrix4D(  w, 0.0, 0.0, 0.0,
                         0.0,  nh, 0.0,   h,
                         0.0, 0.0, 1.0, 0.0,
                         0.0, 0.0, 0.0, 1.0));
    }
};

/**************************** Inline Operators *****************************/
 inline Matrix4D Matrix_Matrix_Multiply(const Matrix4D& a, const Matrix4D& b)
    {
        return (Matrix4D(a(0,0) * b(0,0) + a(0,1) * b(1,0) + a(0,2) * b(2,0) + a(0,3) * b(3,0), 
                         a(0,0) * b(0,1) + a(0,1) * b(1,1) + a(0,2) * b(2,1) + a(0,3) * b(3,1),
                         a(0,0) * b(0,2) + a(0,1) * b(1,2) + a(0,2) * b(2,2) + a(0,3) * b(3,2), 
                         a(0,0) * b(0,3) + a(0,1) * b(1,3) + a(0,2) * b(2,3) + a(0,3) * b(3,3),  
                         a(1,0) * b(0,0) + a(1,1) * b(1,0) + a(1,2) * b(2,0) + a(1,3) * b(3,0),  
                         a(1,0) * b(0,1) + a(1,1) * b(1,1) + a(1,2) * b(2,1) + a(1,3) * b(3,1),  
                         a(1,0) * b(0,2) + a(1,1) * b(1,2) + a(1,2) * b(2,2) + a(1,3) * b(3,2),  
                         a(1,0) * b(0,3) + a(1,1) * b(1,3) + a(1,2) * b(2,3) + a(1,3) * b(3,3),  
                         a(2,0) * b(0,0) + a(2,1) * b(1,0) + a(2,2) * b(2,0) + a(2,3) * b(3,0),  
                         a(2,0) * b(0,1) + a(2,1) * b(1,1) + a(2,2) * b(2,1) + a(2,3) * b(3,1),  
                         a(2,0) * b(0,2) + a(2,1) * b(1,2) + a(2,2) * b(2,2) + a(2,3) * b(3,2),
                         a(2,0) * b(0,3) + a(2,1) * b(1,3) + a(2,2) * b(2,3) + a(2,3) * b(3,3),    
                         a(3,0) * b(0,0) + a(3,1) * b(1,0) + a(3,2) * b(2,0) + a(3,3) * b(3,0), 
                         a(3,0) * b(0,1) + a(3,1) * b(1,1) + a(3,2) * b(2,1) + a(3,3) * b(3,1), 
                         a(3,0) * b(0,2) + a(3,1) * b(1,2) + a(3,2) * b(2,2) + a(3,3) * b(3,2), 
                         a(3,0) * b(0,3) + a(3,1) * b(1,3) + a(3,2) * b(2,3) + a(3,3) * b(3,3)));
    }

    inline Vector3D Matrix_Vector_Multiply(const Matrix4D& m, const Vector3D& v)
    {
        return (Vector3D(m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z,
                         m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z,
                         m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z));
    }

    inline PointH Matrix_PointH_Multiply(const Matrix4D& m, const PointH& p)
    {
        return (PointH(m(0,0) * p.x + m(0,1) * p.y + m(0,2) * p.z + m(0,3) * p.w,
                       m(1,0) * p.x + m(1,1) * p.y + m(1,2) * p.z + m(1,3) * p.w,
                       m(2,0) * p.x + m(2,1) * p.y + m(2,2) * p.z + m(2,3) * p.w,
                       m(3,0) * p.x + m(3,1) * p.y + m(3,2) * p.z + m(3,3) * p.w));
    }

    #endif //__MATRIX4D_H__