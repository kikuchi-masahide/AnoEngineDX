#include "Scene.h"
#include "Layer.h"

Scene::Scene(boost::weak_ptr<Game> const _game)
	:mGame(_game), mIsLayerAddable(true),mThis(boost::weak_ptr<Scene>()) {}

void Scene::Update()
{
	UniqueUpdate();
	CommonUpdate();
}

void Scene::CommonUpdate() {
	mIsLayerAddable = false;
	UpdateLayers();
}

void Scene::UniqueUpdate() {}

void Scene::UpdateLayers()
{
	mIsLayerAddable = true;
	for (int i = 0; i < mLayers.size(); i++)
	{
		mLayers[i]->Update();
	}
}

void Scene::Output()
{
	CommonOutput();
	UniqueOutput();
}

void Scene::UniqueOutput() {}

void Scene::CommonOutput() {
	OutputLayers();
	mIsLayerAddable = true;
	for (int i = 0; i < mPendingLayers.size(); i++) {
		InsertLayer(mPendingLayers[i]);
	}
	mPendingLayers.clear();
}

void Scene::OutputLayers()
{
	for (auto layer : mLayers)
	{
		layer->Output();
	}
}

void Scene::InsertLayer(boost::shared_ptr<Layer> _layer)
{
	int prio = _layer->GetUpdPriority();
	int i = 0;
	for (; i < mLayers.size(); i++) {
		if (mLayers[i]->GetUpdPriority() < prio)break;
	}
	mLayers.insert(mLayers.begin() + i, _layer);
}

void Scene::AddLayer(boost::shared_ptr<Layer> _layer)
{
	if (mIsLayerAddable) {
		mLayers.push_back(_layer);
	}
	else {
		mPendingLayers.push_back(_layer);
	}
}

Scene::~Scene() {}

boost::weak_ptr<Scene> Scene::GetWeakThis() const
{
	return mThis;
}

void Scene::SetWeakThis(boost::shared_ptr<Scene> _this)
{
	if (mThis.expired() != 0)
	{
		Log::OutputCritical("Resetting of weak this pointer");
		assert(0);
	}
	mThis = boost::weak_ptr<Scene>(_this);
}

void Scene::DeleteLayers()
{
	for (int n = 0; n < mLayers.size(); n++) {
		if (mLayers[n]->GetDeleteFlag()) {
			mLayers.erase(mLayers.begin() + n);
			n--;
		}
	}
}