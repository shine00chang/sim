#include "environment.h"
#include "view.h"

std::map<std::string, Environment> EnvironmentLibrary;
std::string DefaultEnv;

void Environment::runControllers (const Application& app, View& view) {
    for (auto& c : m_controllers)
        c(this, app, view);
}
