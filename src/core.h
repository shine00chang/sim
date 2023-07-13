#ifndef __CORE_h__
#define __CORE_h__

#include <iostream>
#include <cmath>

struct Vec2 {
    double x, y;

    Vec2(double x, double y) : x(x), y(y) {}

    const Vec2   operator + (const Vec2& o)  const { return Vec2 (x+o.x, y+o.y); }
    const Vec2   operator - (const Vec2& o)  const { return Vec2 (x-o.x, y-o.y); }
    const Vec2   operator * (const double o) const { return Vec2 (x*o, y*o); }
    const double operator * (const Vec2& o)  const { return x*o.x + y*o.y; }

    bool isNorm () const { return x * x + y * y == 1; };
    Vec2* normalize () {
        double mag = std::sqrt(x * x + y * y);
        x /= mag; 
        y /= mag;
        return this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2& o) { os << "(" << o.x << ", " << o.y << ")"; return os; }
};

#endif
