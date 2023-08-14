#ifndef __ENVIRONMENT_h__
#define __ENVIRONMENT_h__

#include "controller.h"
#include "body.h"

#include <vector>
#include <map>
#include <string>


constexpr double GRAVITY = -40;

class Application;
class Environment {
    std::vector<Body> m_bodies;
    std::vector<EnvController> m_controllers;


public: 
     Environment() {};
    ~Environment() {};

    void collide (const double dt);
    void runControllers (const Application& app);

    Body* addBody      (const Body& body) { m_bodies.push_back(body); return &m_bodies.back(); } 
    void addController (const EnvController controller) { m_controllers.push_back(controller); }

    std::vector<Body>& getBodiesMut() { return m_bodies; }
    const std::vector<Body>& getBodies() const { return m_bodies; }
};

extern std::map<std::string, Environment> EnvironmentLibrary;
void makeTestEnvs ();


#endif
