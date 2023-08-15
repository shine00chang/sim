#include "environment.h"

std::map<std::string, Environment> EnvironmentLibrary;
std::string DefaultEnv;

void Environment::runControllers (const Application& app) {
    for (auto& c : m_controllers)
        c(this, app);
}
