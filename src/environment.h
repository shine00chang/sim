#ifndef __ENVIRONMENT_h__
#define __ENVIRONMENT_h__

#include "body.h"
#include "controller.h"

#include <vector>
#include <map>
#include <string>


constexpr double GRAVITY = -40;

class Environment {
    std::vector<Body> m_bodies;
    std::vector<Controller> m_controllers;


public: 
     Environment() {};
    ~Environment() {};

    void collide (const double dt);
    void runControllers ();

    Body* addBody      (const Body& body) { m_bodies.push_back(body); return &m_bodies.back(); } 
    void addController (const Controller controller) { m_controllers.push_back(controller); }

    std::vector<Body>& getBodiesMut() { return m_bodies; }
    const std::vector<Body>& getBodies() const { return m_bodies; }
};

extern std::map<std::string, Environment> EnvironmentLibrary;
void makeTestEnvs ();


#endif
