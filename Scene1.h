#pragma once
#include "Scene.h"
#include "Component1.h"
#include "Component2.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "ComponentHandle.h"

class Scene1 :public Scene
{
public:
	Scene1(Game* _game)
		:Scene(_game)
	{
		Log::OutputTrivial("Scene1 Initializer");
		auto handle = AddObject(GetVector2(0,0),1.0,0.0);
		handle->AddOutputComponent<Component1>(10,0);
		//handle->AddOutputComponent<Component1>(60,1);
		//handle->AddUpdateComponent<Component2>(20, 1);
		//handle->AddUpdateComponent<Component2>(60, 0);
	}
	void UniqueOutput()
	{
		Log::OutputTrivial("Scene1 UniqueOutput");
	}
	void UniqueUpdate()
	{
		Log::OutputTrivial("Scene1 UniqueUpdate");
	}
	~Scene1()
	{
		Log::OutputTrivial("Scene1 destructor");
	}
};