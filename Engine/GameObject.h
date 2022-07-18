//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Scene;
class Game;
#include "ComponentHandle.h"

/// <summary>
/// �V�[���Ɋ܂܂��I�u�W�F�N�g�B�قڎqcomponent���܂Ƃ߂�@�\�̂�
/// </summary>
class GameObject final {
public:
	/// <param name="comphandle_reserve_num">�qcomponent��handle��ێ�����vector�̏���reserve��</param>
	GameObject(int id, int comphandle_reserve_num);
	//Scene��AddUpdate/OutputComponent����Ă΂��
	void AddComponent(ComponentHandle<Component> comp);
	//�qComponent�̓������ς݂̂��̂�o�^��������(Scene::ProcessPandingComps�p)
	void UnregisterInvalidChilds();
	//���ׂĂ̎qcomponent��delete flag��t����
	void SetAllCompsFlag();
	~GameObject();
private:
	const int kObjId;
	//HACK:�v�[���g����?
	std::vector<ComponentHandle<Component>> comps_;
};

/// <summary>
/// GameObject���w���n���h���B����I�u�W�F�N�g�ɑ΂��鑀����s���ꍇ�ɗp����
/// </summary>
using GameObjectHandle = unsigned int;
