#ifndef __CORE_h__
#define __CORE_h__

#include <iostream>
#include <cmath>

struct Vec2 {
    double x, y;

    Vec2(double x, double y) : x(x), y(y) {}

    inline Vec2   operator + (const Vec2& o)  const { return Vec2 (x+o.x, y+o.y); }
    inline Vec2   operator - ()               const { return Vec2 (-x, -y); };
    inline Vec2   operator - (const Vec2& o)  const { return Vec2 (x-o.x, y-o.y); }
    inline Vec2   operator * (const double o) const { return Vec2 (x*o, y*o); }
    inline double operator * (const Vec2& o)  const { return x*o.x + y*o.y; }

    inline bool isNorm () const { return x * x + y * y == 1; };
    Vec2& normalize () {
        double mag = std::sqrt(x * x + y * y);
        x /= mag; 
        y /= mag;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2& o) { os << "(" << o.x << ", " << o.y << ")"; return os; }
};

struct Vec3 {
    double x, y, z;

    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    inline Vec3   operator + (const Vec3& o)  const { return Vec3 (x+o.x, y+o.y, z+o.z); }
    inline Vec3   operator - ()               const { return Vec3 (-x, -y, -z); };
    inline Vec3   operator - (const Vec3& o)  const { return Vec3 (x-o.x, y-o.y, z-o.z); }
    inline Vec3   operator * (const double o) const { return Vec3 (x*o, y*o, z*o); }
    inline double operator * (const Vec3& o)  const { return x*o.x + y*o.y + z*o.z; }
    Vec3   operator ^ (const Vec3& o)  const {
        return Vec3(
            y * o.z - z * o.y,
          -(x * o.z - z * o.x),
            x * o.y - y * o.x
        );
    }

    inline bool isNorm () const { return x * x + y * y + z * z == 1; };
    Vec3& normalize () {
        double mag = std::sqrt(x * x + y * y + z * z);
        x /= mag; 
        y /= mag;
        z /= mag;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& o) { os << "(" << o.x << ", " << o.y << ", " << o.z << ")"; return os; }
};

#endif
