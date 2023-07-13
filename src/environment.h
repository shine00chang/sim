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
    Body* addBody(const Body& body) { m_bodies.push_back(body); return &m_bodies.back(); } 

    std::vector<Body>& getBodiesMut() { return m_bodies; }
    const std::vector<Body>& getBodies() const { return m_bodies; }

};

#endif
