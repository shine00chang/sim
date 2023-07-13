/*
#ifndef __CONTROLLER_h__
#define __CONTROLLER_h__

#include "body.h"
#include <vector>

class Application;
class Environment;
class Controller {
    void (*m_run)(Body* body, const Application&, const Environment&);

public:
    Controller (
            Body* body,
            void (*run)(Body* body, const Application&, const Environment&)
        ) : 
        m_body(body),
        m_run(run)
        {};

    void run (const Application& app, const Environment& env) const {
        if (m_body == nullptr) 
            throw "controller's assigned body was deallocated.";

        m_run(m_body, app, env);
    }
};
using Controllers = std::vector<Controller>;

#endif
*/
