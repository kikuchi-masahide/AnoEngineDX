#pragma once
#include "Component.h"
#include "ComponentHandle.h"

//test update-component
class Component2 :Component {
public:
	Component2(GameObject* _owner, boost::shared_ptr<std::set<ComponentHandle*>> _hset, int _lim, int _id)
		:Component(_owner, _hset, _id), mID(_id), mLimit(_lim),mCounter(0)
	{
		std::string str("Component2 Initialization(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputTrivial(str);
	}
	~Component2() {
		std::string str("Component2 Delete(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputTrivial(str);
	}
	virtual void Update() {
		mCounter++;
		if (mCounter == mLimit) {
			std::string str("Timer! from Component2(id=");
			str += std::to_string(mID);
			str += ",mCounter=";
			str += std::to_string(mCounter);
			str += ")";
			Log::OutputCritical(str);
		}
		std::string str("Component1 Update(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputTrivial(str);
	}
	int mID;
	int mCounter;
	int mLimit;
};