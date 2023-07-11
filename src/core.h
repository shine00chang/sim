#ifndef __CORE_h__
#define __CORE_h__

struct Vec2 {
    double x, y;

    Vec2(double x, double y) : x(x), y(y) {}

    const Vec2   operator + (const Vec2& o)  const { return Vec2 (x+o.x, y+o.y); }
    const Vec2   operator * (const double o) const { return Vec2 (x*o, y*o); }
    const double operator * (const Vec2& o)  { return x*o.x + y+o.y; }
};

#endif
