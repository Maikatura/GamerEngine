#include "pch.h"
#include "application.h"

int main(void)
{
  auto app = std::make_unique<Application>("Gamer Engine");
  app->loop();

  return 0;
}
