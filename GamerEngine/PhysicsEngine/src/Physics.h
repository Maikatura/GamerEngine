#pragma once
#include <memory>


class Physics
{
public:

	Physics();
	~Physics();

	static Physics& Get();

	void Init();

	void Update();


private:
};

