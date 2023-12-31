#ifndef __CORE_h__
#define __CORE_h__

#include <iostream>
#include <cmath>

#define EPSILON 0.00001

struct Vec2 {
    double x, y;

    Vec2(double x, double y) : x(x), y(y) {}

    inline Vec2   operator + (const Vec2& o)  const { return Vec2 (x+o.x, y+o.y); }
    inline Vec2   operator - ()               const { return Vec2 (-x, -y); };
    inline Vec2   operator - (const Vec2& o)  const { return Vec2 (x-o.x, y-o.y); }
    inline Vec2   operator * (const double o) const { return Vec2 (x*o, y*o); }
    inline double operator * (const Vec2& o)  const { return x*o.x + y*o.y; }
    inline bool   operator ==(const Vec2& o)  const { return std::fabs(x - o.x) < EPSILON && std::fabs(y - o.y) < EPSILON; }

    inline double mag () const { return std::sqrt(x*x + y*y); }
    inline bool isNorm () const { return std::fabs(x * x + y * y - 1) < EPSILON; };
    Vec2 norm () const {
        double m = mag();
        return Vec2(x / m, y / m);
    }
    Vec2& normalize () {
        *this = norm();
        return *this;
    }

    Vec2 rotate (double rad) {
        double c = std::cos(rad);
        double s = std::sin(rad);
        return Vec2(c*x - s*y,
                    s*x + c*y);
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
    inline bool   operator ==(const Vec3& o)  const { return x == o.x && y == o.y && z == o.z; }

    Vec3   operator ^ (const Vec3& o)  const {
        return Vec3(
            y * o.z - z * o.y,
          -(x * o.z - z * o.x),
            x * o.y - y * o.x
        );
    }

    inline bool isNorm () const { return std::fabs( x * x + y * y + z * z - 1) < EPSILON; };
    Vec3 norm () const {
        double m = mag();
        return Vec3(
                x / m,
                y / m,
                z / m);
    }
    Vec3& normalize () {
        *this = norm();
        return *this;
    }

    inline double mag () const { return std::sqrt(x*x + y*y + z*z); }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& o) { os << "(" << o.x << ", " << o.y << ", " << o.z << ")"; return os; }
};



#endif
