#include "app.h"
#include "environment.h"

#include <ostream>

extern void setEnvs();

int main( int argc, char* args[] )
{
    // Make environments
    setEnvs();

    if (EnvironmentLibrary.empty()) 
    {
        std::cout << "No environments defined.\n";
        return 1;
    }

    // Show environments
    std::cout << "Available Environments:\n";
    for (const auto& [k, _] : EnvironmentLibrary) 
    {
        std::cout << " -  " << k << std::endl;
    }

    // Get Environment argument
    std::string envName = DefaultEnv;
    if (DefaultEnv.empty()) 
    {
        std::cout << "Default Environment is not set, using first environment in library\n";
        envName = EnvironmentLibrary.begin()->first;
    }

    if (argc >= 2) 
    {
        envName = std::string(args[1]);
    }

    if (EnvironmentLibrary.count(envName) == 0) 
    {
        std::cout << "Could not find environment '" << envName << "'" << std::endl;
        return 1;
    }

    Application* app;

    try {
        app = new Application();

    } catch (std::string err) {
        std::cout << "App initialization failed with: '" << err << "'" << std::endl;
        return 1;
    }

    std::cout << "Using environment '" << envName << "'" << std::endl;

    Environment env = EnvironmentLibrary[envName];
    app->start(std::move(env));

    return 0;
}
