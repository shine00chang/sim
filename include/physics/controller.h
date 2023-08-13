#ifndef __CONTROLLER_h__
#define __CONTROLLER_h__

class Body;
class Environment;
class Application;

using Controller = void (*)(Body* body, const Application& app);
using EnvController = void (*)(Environment* env, const Application& app);

#endif
