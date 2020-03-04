#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include "math.h"

struct Vector3D
{
    double x, y, z;

    Vector3D() = default;

    Vector3D(double a, double b, double c)
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

    Vector3D& operator *=(double s)
    {
        x *= s;
        y *= s;
        z *= s;
        return (*this);
    }

    Vector3D& operator /=(double s)
    {
        s = 1.0f / s;
        x *= s;
        y *= s;
        z *= s;
        return (*this);
    }

    Vector3D& operator +=(const Vector3D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }

    Vector3D operator -=(const Vector3D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }

    Vector3D& operator =(const Vector3D& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return (*this);
    }
};

inline Vector3D operator +(const Vector3D& a, const Vector3D& b)
{
    return (Vector3D(a.x + b.x, a.y + b.y, a.z + b.z));
}

inline Vector3D operator -(const Vector3D& a, const Vector3D& b)
{
    return (Vector3D(a.x - b.x, a.y - b.y, a.z - b.z)); 
}

inline Vector3D operator *(const Vector3D& v, double s)
{
    return (Vector3D(v.x * s, v.y * s, v.z * s));
}

inline Vector3D operator /(const Vector3D& v, double s)
{
    s = 1.0f / s;
    return (Vector3D(v.x * s, v.y * s, v.z * s));
}

inline Vector3D operator -(const Vector3D& v)
{
    return (Vector3D(-v.x, -v.y, -v.z));
}

inline double Magnitude(const Vector3D& v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline Vector3D Normalize(const Vector3D& v)
{
    return (v / Magnitude(v));
}

#endif /*__VECTOR3D_H__*/