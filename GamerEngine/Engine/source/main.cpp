#include "pch.h"
#include "Application.h"

int main(void)
{
	auto app = std::make_unique<Application>();

	app->Init("GamerEngines CHADITOR", 0, 0, 2);
	app->Loop();

	return 0;
}
