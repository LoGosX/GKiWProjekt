#include "GameEngine.h"


int main(void)
{
	auto engine = new GameEngine;
	engine->initialize();
	engine->run();
	delete engine;
}
