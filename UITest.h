#pragma once
#include "UIScreen.h"

class UITest :public UIScreen
{
public:
	UITest()
		:UIScreen(false, false)
	{
		Log::OutputTrivial("UITest constructor");
		mCounter = 0;
	}
	virtual void Update()
	{
		mCounter++;
		if (mCounter == 300)
		{
			SetDeleteFlag();
		}
	}
	virtual void Output()
	{
		std::string str("");
		str += std::to_string(mCounter);
		Log::OutputTrivial(str);
	}
	virtual ~UITest()
	{
		Log::OutputCritical("UITest destructor");
	}
private:
	int mCounter;
};