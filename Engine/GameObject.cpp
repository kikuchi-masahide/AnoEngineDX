#include "GameObject.h"
#include "GameObjectHandle.h"
#include "Component.h"
#include "Scene.h"
#include "Game.h"

GameObject::GameObject(Scene* const scene, MatVec::Vector2 pos, double scale, double angle)
	:scene_(scene), position_(pos), scale_(scale), rotation_(angle), delete_flag_(false)
{
}

MatVec::Vector2 GameObject::GetPosition() const
{
	return position_;
}

MatVec::Vector2 GameObject::SetPosition(MatVec::Vector2 pos)
{
	position_ = pos;
	return position_;
}

double GameObject::GetScale() const
{
	return scale_;
}

double GameObject::SetScale(double sc)
{
	if (sc < 0) {
		scale_ = 0.0f;
	}
	else {
		scale_ = sc;
	}
	return scale_;
}

double GameObject::GetRotation() const
{
	return rotation_;
}

double GameObject::SetRotation(double ro)
{
	rotation_ = ro;
	return rotation_;
}

bool GameObject::GetDeleteFlag() const
{
	return delete_flag_;
}

void GameObject::SetDeleteFlag()
{
	delete_flag_ = true;
}

Game& GameObject::GetGame()
{
	return scene_->game_;
}

GameObject::~GameObject() {
	//自分を消す前にコンポーネントたちを消しておく
	for (auto itr = update_components_.begin(); itr != update_components_.end(); itr++) {
		DeleteComponent(*itr);
	}
	for (auto itr = output_components_.begin(); itr != output_components_.end(); itr++) {
		DeleteComponent(*itr);
	}
	//自分を指すGameObjectHandleをReset
	while (!handles_.empty()) {
		auto itr = handles_.begin();
		(*itr)->Reset();
	}
}

void GameObject::AddUpdateComponentToScene(ComponentHandle<Component> handle)
{
	scene_->AddUpdateComponent(this, handle);
}

void GameObject::AddOutputComponentToScene(ComponentHandle<Component> handle)
{
	scene_->AddOutputComponent(this, handle);
}

GameObjectHandle GameObject::This()
{
	return GameObjectHandle(this, &handles_);
}

void GameObject::DeleteComponent(Component* component)
{
	component->delete_check_ = true;
	delete component;
}

void GameObject::DeleteFlagedComponents()
{
	//コンポーネントを巡回しフラグが立っているものを削除
	auto itr = update_components_.begin();
	while (itr != update_components_.end()) {
		if ((*itr)->GetDeleteFlag()) {
			DeleteComponent(*itr);
			itr = update_components_.erase(itr);
		}
		else {
			itr++;
		}
	}
	itr = output_components_.begin();
	while (itr != output_components_.end()) {
		if ((*itr)->GetDeleteFlag()) {
			DeleteComponent(*itr);
			itr = output_components_.erase(itr);
		}
		else {
			itr++;
		}
	}
}