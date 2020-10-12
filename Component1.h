#pragma once
#include "Component.h"

class Component1 : public Component
{
public:
	Component1(boost::weak_ptr<GameObject> _owner)
		:Component(_owner)
	{
		//Log::OutputTrivial("Component1 Initialization");
	}
	virtual void Update()
	{
		//Log::OutputTrivial("Component1 Update");
	}
	~Component1()
	{
		Log::OutputTrivial("Component1 Destructor");
	}
};