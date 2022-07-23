//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Scene;
class Game;
#include "ComponentHandle.h"
#include "boost/pool/pool_alloc.hpp"

/// /// <summary>
/// �V�[���Ɋ܂܂��I�u�W�F�N�g�B�قڎqcomponent���܂Ƃ߂�@�\�̂�
/// </summary>
class GameObject final {
public:
	/// <param name="comphandle_reserve_num">�qcomponent��handle��ێ�����vector�̏���reserve��</param>
	GameObject(int id);
	//Scene��AddUpdate/OutputComponent����Ă΂��
	void AddComponent(ComponentHandle<Component> comp);
	//�qComponent�̓������ς݂̂��̂�o�^��������(Scene::ProcessPandingComps�p)
	void UnregisterInvalidChilds();
	//���ׂĂ̗L���Ȏqcomponent��delete flag��t����
	void SetAllCompsFlag();
	~GameObject();
private:
	const int kObjId;
	struct myallocator {
	public:
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		static char* malloc(const size_type size) {
			return DBG_NEW char[size];
		}
		static void free(char* const p) {
			delete p;
		}
	};
	//HACK:�q��GameObject���c������`�ɂ���Ȃ�΂��ꂪ�ő�
	//����𖳂�����΁A�{����GameObject���C���X�^���X�Ƃ��Ď��K�v�͂Ȃ��Ȃ胁�������������A�Ƃ肠��������͌�X�l����
	std::list<ComponentHandle<Component>, boost::fast_pool_allocator<ComponentHandle<Component>, myallocator>> comps_;
};

/// <summary>
/// GameObject���w���n���h���B����I�u�W�F�N�g�ɑ΂��鑀����s���ꍇ�ɗp����
/// </summary>
using GameObjectHandle = unsigned int;
