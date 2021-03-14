#pragma once
#include "Component.h"
#include "Layer2DComponent.h"
#include "ComponentHandle.h"
#include "Game.h"

class InputMonitorComponent :public Component
{
public:
	InputMonitorComponent(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, ComponentHandle<Layer2DComponent> _layer2d)
		:Component(_owner, _hset, 10), mLayer2d(_layer2d)
	{
		Log::OutputTrivial("InputMonitorComponent Constructor");
	}
	virtual ~InputMonitorComponent()
	{
		Log::OutputTrivial("InputMonitorComponent Destructor");
	}
	virtual void Update()
	{
		static double deg = 3.141592 * 2 / 120;
		auto leftstate = GetGame().mInputSystem.GetKeyState('A');
		static double curdeg = 0;
		if (leftstate == ButtonState::Held)
		{
			mLayer2d->RotateRect(GetVector3(0, 0, 1), deg);
			curdeg += deg;
		}
		else if (leftstate == ButtonState::Pressed)
		{
			mLayer2d->RotateRect(GetVector3(0, 0, 1), -curdeg);
			curdeg = 0;
		}
		else if (leftstate == ButtonState::Released)
		{
			Log::OutputTrivial("Released");
		}
		auto mousemove = GetGame().mInputSystem.GetMouseMove();
		//mLayer2d->TranslationRect(boost::qvm::XY0(mousemove));
	}
	ComponentHandle<Layer2DComponent> mLayer2d;
};