#ifndef __POINT_H__
#define __POINT_H__

#include "Vector3D.h"

struct Point
{
    double x, y, z;

    Point() = default;

    Point(double a, double b, double c)
    {
        x = a; 
        y = b;
        z = c;
    }

    double& operator [](int i)
    {
        return ((&x)[i]);
    }

    const double& operator [](int i) const
    {
        return ((&x)[i]);
    }

    Point& operator *=(double s)
    {
        x *= s;
        y *= s;
        z *= s;
        return (*this);
    }

     Point& operator /=(double s)
    {
        s = 1.0f / s;
        x *= s;
        y *= s;
        z *= s;
        return (*this);
    }

    Point& operator +=(const Point& p)
    {
        x += p.x;
        y += p.y;
        z += p.z;
        return (*this);
    }

    Point operator -=(const Point& p)
    {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return (*this);
    }

    Point& operator =(const Point& p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
        return (*this);
    }

    Vector3D to_vector()
    {
        return (Vector3D(x, y, z));
    }
};

inline Point Point_Point_Add(const Point& a, const Point& b)
{
    return (Point(a.x + b.x, a.y + b.y, a.z + b.z));
}

inline Point Point_Point_Subtract(const Point& a, const Point& b)
{
    return (Point(a.x - b.x, a.y - b.y, a.z - b.z)); 
}

inline Point Point_Scale_Multiply(const Point& p, double s)
{
    return (Point(p.x * s, p.y * s, p.z * s));
}

inline Point Point_Scale_Divide(const Point& p, double s)
{
    s = 1.0f / s;
    return (Point(p.x * s, p.y * s, p.z * s));
}

inline Point Point_Negate(const Point& p)
{
    return (Point(-p.x, -p.y, -p.z));
}


#endif /*__POINT_H__*/