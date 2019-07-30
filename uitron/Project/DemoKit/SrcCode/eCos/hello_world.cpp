extern "C" {
#include "SysKer.h"
#include "Debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "hello_world.h"
}


class HelloWorld
{
public:
	HelloWorld();
	~HelloWorld();
};

HelloWorld::HelloWorld()
{
	debug_msg((char *)"HelloWorld::HelloWorld()");
}

HelloWorld::~HelloWorld()
{
	debug_msg((char *)"HelloWorld::~HelloWorld()");
}

extern "C" void ecos_sample_cpp_api(void)
{
    HelloWorld* p = new HelloWorld;
	delete p;
}


