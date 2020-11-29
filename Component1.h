#pragma once
#include "Component.h"

//test output-component
class Component1 : public Component
{
public:
	Component1(GameObject* _owner, boost::shared_ptr<std::set<ComponentHandle*>> _hset, int _lim, int _id)
		:Component(_owner, _hset,_id),mCounter(0),mLimit(_lim),mID(_id)
	{
		std::string str("Component1 Initialization(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputTrivial(str);
	}
	virtual void Update()
	{
		mCounter++;
		if (mCounter == mLimit) {
			SetDeleteFlag();
		}
		std::string str("Component1 Update(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputTrivial(str);
	}
	~Component1()
	{
		std::string str("Component1 Delete(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputCritical(str);
	}
	int mCounter;
	int mLimit;
	int mID;
};