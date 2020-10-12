#pragma once
#include "Layer.h"
#include "Object1.h"
#include "Component1.h"

class Layer1 :public Layer
{
public:
	Layer1(boost::weak_ptr<Scene> _scene)
		:Layer(_scene, Rect2(0, 100, 0, 100))
	{
		//Log::OutputTrivial("Layer1 Initialization");
	}
	void Set()
	{
		boost::shared_ptr<GameObject1> obj(new GameObject1(mThis));
		obj->SetWeakThis(obj);
		boost::shared_ptr<Component1> comp(new Component1(obj));
		comp->SetWeakThis(comp);
		obj->AddComponent(comp);
		AddObject(obj);
	}
	void UniqueUpdate()
	{
		//Log::OutputTrivial("Layer1 UniqueUpdate");
	}
	void UniqueOutput()
	{
		//Log::OutputTrivial("Layer1 UniqueOutput");
	}
	~Layer1()
	{
		Log::OutputTrivial("Layer1 Destructor");
	}
};