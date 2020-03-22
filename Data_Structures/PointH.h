#ifndef __POINTH_H__
#define __POINTH_H__

struct PointH
{
    double x, y, z, w;

    PointH() = default;

    PointH(double a, double b, double c, double d)
    {
        x = a; 
        y = b;
        z = c;
        w = d;
    }

    double& operator [](int i)
    {
        return ((&x)[i]);
    }

    const double& operator [](int i) const
    {
        return ((&x)[i]);
    }

    PointH& operator *=(double s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return (*this);
    }

     PointH& operator /=(double s)
    {
        s = 1.0f / s;
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return (*this);
    }

    PointH& operator +=(const PointH& p)
    {
        x += p.x;
        y += p.y;
        z += p.z;
        w += p.w;
        return (*this);
    }

    PointH operator -=(const PointH& p)
    {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        w -= p.w;
        return (*this);
    }

    PointH& operator =(const PointH& p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
        w = p.w;
        return (*this);
    }
};

inline PointH operator +(const PointH& a, const PointH& b)
{
    return (PointH(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w));
}

inline PointH operator -(const PointH& a, const PointH& b)
{
    return (PointH(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w)); 
}

inline PointH operator *(const PointH& p, double s)
{
    return (PointH(p.x * s, p.y * s, p.z * s, p.w * s));
}

inline PointH operator /(const PointH& p, double s)
{
    s = 1.0f / s;
    return (PointH(p.x * s, p.y * s, p.z * s, p.w * s));
}

inline PointH operator -(const PointH& p)
{
    return (PointH(-p.x, -p.y, -p.z, -p.w));
}


#endif /*__POINTH_H__*/