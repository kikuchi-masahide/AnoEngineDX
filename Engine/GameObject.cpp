//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject(int id)
	:kObjId(id)
{
}

void GameObject::AddComponent(ComponentHandle<Component> comp)
{
	comps_.push_back(comp);
}
void GameObject::UnregisterInvalidChilds()
{
	std::erase_if(comps_, [](const ComponentHandle<Component>& comp) {
		return !comp;
	});
}

void GameObject::SetAllCompsFlag()
{
	std::for_each(comps_.begin(), comps_.end(), [](ComponentHandle<Component>& comp) {
		comp->SetDeleteFlag();
	});
}

GameObject::~GameObject()
{
	comps_.clear();
}
