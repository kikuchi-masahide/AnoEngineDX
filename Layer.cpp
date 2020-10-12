#include "Layer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Game.h"

Layer::~Layer() {}

void Layer::SetWeakThis(boost::shared_ptr<Layer>& _this)
{
	if (mThis.expired() != 0)
	{
		Log::OutputCritical("Resetting of weak this pointer");
		assert(0);
	}
	mThis = boost::weak_ptr<Layer>(_this);
}

boost::weak_ptr<Layer> Layer::GetWeakThis() const
{
	return mThis;
}

boost::weak_ptr<Game> Layer::GetGame() const
{
	boost::shared_ptr<Scene> scene(mScene);
	return scene->mGame;
}

int Layer::GetUpdPriority() const
{
	return mUpdPriority;
}

Rect2 Layer::GetClientPosition() const
{
	return mRect;
}

void Layer::SetClientPosition(Rect2 _rect)
{
	mRect = _rect;
}

void Layer::Update()
{
	CommonUpdate();
	UniqueUpdate();
}

void Layer::CommonUpdate() {
	mIsObjAddable = false;
	UpdateObjects();
}

void Layer::UniqueUpdate() {}

void Layer::UpdateObjects() {
	for (int n = 0; n < mObjects.size(); n++) {
		mObjects[n]->Update();
	}
}

void Layer::Output() {
	CommonOutput();
	UniqueOutput();
}

void Layer::CommonOutput() {
	OutputObjects();
	mIsObjAddable = true;
	for (int n = 0; n < mPandingObjects.size(); n++) {
		InsertObject(mPandingObjects[n]);
	}
	mPandingObjects.clear();
}

void Layer::UniqueOutput() {}

void Layer::OutputObjects()
{
	for (int n = 0; n < mObjects.size(); n++) {
		mObjects[n]->Output();
	}
}

void Layer::InsertObject(boost::shared_ptr<GameObject> _obj)
{
	int objpriority = _obj->GetUpdPriority();
	//v[s].mPriority >= _obj.mPriority > v[e].mPriority
	int s = -1, e = mObjects.size();
	while (e - s > 1) {
		int m = (e + s) / 2;
		if (mObjects[m]->GetUpdPriority() >= objpriority)s = m;
		else e = m;
	}
	mObjects.insert(mObjects.begin() + e, _obj);
}

void Layer::DeleteObjects()
{
	for (int n = 0; n < mObjects.size(); n++) {
		if (mObjects[n]->GetDeleteFlag()) {
			mObjects.erase(mObjects.begin() + n);
			n--;
		}
	}
}

Layer::Layer(boost::weak_ptr<Scene> const _scene, Rect2 _rect, int _order)
	:mScene(_scene), mRect(_rect), mUpdPriority(_order), mIsObjAddable(true)
	,mThis(boost::weak_ptr<Layer>()) {}

void Layer::AddObject(boost::shared_ptr<GameObject> _obj)
{
	if (mIsObjAddable)
	{
		InsertObject(_obj);
	}
	else {
		mPandingObjects.push_back(_obj);
	}
}