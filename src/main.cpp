#include "app.h"
#include "environment.h"

#include <ostream>


int main( int argc, char* args[] )
{
    // Make environments
    makeTestEnvs();

    // Get Environment argument
    std::string envName = "rainyDay";
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

    Environment env = EnvironmentLibrary[envName];
    app->start(std::move(env));

    return 0;
}
