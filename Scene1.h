#pragma once
#include "Scene.h"
#include "Layer1.h"

class Scene1 :public Scene
{
public:
	Scene1(boost::weak_ptr<Game> _game)
		:Scene(_game)
	{
		//Log::OutputTrivial("Scene1 Initializer");
	}
	void Set()
	{
		boost::shared_ptr<Layer1> layer(new Layer1(mThis));
		layer->SetWeakThis(layer);
		layer->Set();
		AddLayer(layer);
	}
	void UniqueOutput()
	{
		//Log::OutputTrivial("Scene1 UniqueOutput\n");
	}
	void UniqueUpdate()
	{
		//Log::OutputTrivial("Scene1 UniqueUpdate");
	}
	~Scene1()
	{
		Log::OutputTrivial("Scene1 destructor");
	}
};