#pragma once
#include "Scene.h"
#include "Component1.h"
#include "Component2.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "ComponentHandle.h"
#include "Layer2D.h"
#include "Layer2DComponent.h"

class Scene1 :public Scene
{
public:
	Scene1(Game* _game)
		:Scene(_game)
	{
		auto objhandle = AddObject(GetVector2(0, 0), 1.0, 0.0);
		auto layerhandle = AddLayer<Layer2D>(Rect2(0, 400, 0, 400),0,0);
		//AddLayer<Layer2D>(Rect2(512, 1024, 0, 768), 1, 0);
		mlayer2dcomp = objhandle->AddOutputComponent<Layer2DComponent>(layerhandle, 0, 0, Rect2(100, 300, 100, 300));
	}
	void UniqueOutput()
	{
	}
	void UniqueUpdate()
	{
		double deg = 3.141592 * 2 / 120;
		mlayer2dcomp->RotateRect(GetVector3(0, 0, 1), deg);
		Log::OutputTrivial("Scene1 UniqueUpdate");
	}
	~Scene1()
	{
		Log::OutputTrivial("Scene1 destructor");
	}
private:
	ComponentHandle<Layer2DComponent> mlayer2dcomp;
};