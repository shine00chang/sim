#ifndef __BODY_h__
#define __BODY_h__

#include "core.h"

#include <vector>


class Body {
public: 
    Vec2 min;
    Vec2 max;

     Body(double x, double y, double w, double h) : min(x,y), max(x+w, y+h) {};
    ~Body() {};
};

#endif
