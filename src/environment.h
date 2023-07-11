#ifndef __ENVIRONMENT_h__
#define __ENVIRONMENT_h__

#include "body.h"

#include <vector>


class Environment {
    std::vector<Body> m_bodies;

public: 
     Environment() {};
    ~Environment() {};

    void collide();
    void addBody(const Body& body) { m_bodies.push_back(body); }  

    const std::vector<Body>& bodies() const { return m_bodies; }

};

#endif
