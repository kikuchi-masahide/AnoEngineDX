#pragma once
#include "Scene.h"
#include "Component1.h"
#include "Component2.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "ComponentHandle.h"
#include "Layer2D.h"
#include "Layer2DComponent.h"
#include "InputMonitorComponent.h"

class Scene1 :public Scene
{
public:
	Scene1(Game* _game)
		:Scene(_game)
	{
		auto objhandle = AddObject(MatVec::Vector2(0, 0), 1.0, 0.0);
		LayerHandle<Layer2D> layerhandle = AddLayer<Layer2D>(Rect2(0, 1024, 0, 768),0,114514);
		//AddLayer<Layer2D>(Rect2(512, 1024, 0, 768), 1, 0);
		mlayer2dcomp = objhandle->AddOutputComponent<Layer2DComponent>(layerhandle, 0, 0, Rect2(100, 300, 100, 300));
		auto inputmonitor = objhandle->AddUpdateComponent<InputMonitorComponent>(mlayer2dcomp);
	}
	void UniqueOutput()
	{
	}
	void UniqueUpdate()
	{
	}
	~Scene1()
	{
	}
private:
	ComponentHandle<Layer2DComponent> mlayer2dcomp;
};