#pragma once
#include "Component.h"
#include "ComponentHandle.h"
#include "GameObject.h"
#include "GameObjectHandle.h"

//test update-component
class Component2 : public Component {
public:
	Component2(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, int _lim, int _id, GameObjectHandle _handle)
		:Component(_owner, _hset, _id), mCounter(0), mLimit(_lim), mID(_id),mOwner(_handle) {
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
			mOwner->AddOutputComponent<Component1>(120, mID+100);
		}
		std::string str("Component2 Update(id=");
		str += std::to_string(mID);
		str += ")";
		Log::OutputTrivial(str);
	}
	int mID;
	int mCounter;
	int mLimit;
	GameObjectHandle mOwner;
};