#ifndef __CONTROLLER_h__
#define __CONTROLLER_h__

class Body;
class Application;
using Controller = void (*)(Body* body, const Application& app);

#endif
