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
		auto layer2dcenter = (mLayer2d->mVerts[0] + mLayer2d->mVerts[1] + mLayer2d->mVerts[2] + mLayer2d->mVerts[3]) / 4;
		auto mousepos2 = GetGame().mInputSystem.GetMouseClientPos(114514);
		MatVec::Vector3 mousepos3(mousepos2(0), mousepos2(1), 0);
		mLayer2d->TranslationRect(mousepos3 - layer2dcenter);
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
			mLayer2d->RotateRect(MatVec::Vector3(0, 0, 1), deg);
			curdeg += deg;
		}
		else if (leftstate == ButtonState::Pressed)
		{
			mLayer2d->RotateRect(MatVec::Vector3(0, 0, 1), -curdeg);
			curdeg = 0;
		}
		else if (leftstate == ButtonState::Released)
		{
			Log::OutputTrivial("Released");
		}
		auto mousemove = GetGame().mInputSystem.GetMouseMove();
		mLayer2d->TranslationRect(MatVec::Vector3(mousemove(0),mousemove(1),0));
	}
	ComponentHandle<Layer2DComponent> mLayer2d;
};