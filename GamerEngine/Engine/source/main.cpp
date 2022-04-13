#include "pch.h"
#include "Application.h"

int main(void)
{
	auto app = std::make_unique<Application>("Gamer Engine");
	app->Loop();

	return 0;
}
