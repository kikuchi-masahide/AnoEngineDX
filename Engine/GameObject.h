//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Game;
#include "Scene.h"
#include "ComponentHandle.h"
#include "boost/pool/pool_alloc.hpp"

/// /// <summary>
/// �V�[���Ɋ܂܂��I�u�W�F�N�g�B�قڎqcomponent���܂Ƃ߂�@�\�̂�
/// </summary>
class GameObject final {
public:
	Scene* const scene_;
	GameObject(Scene* scene);
	~GameObject();
	/// <summary>
	/// UpdateComponent��ǉ�
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(Args... args);
	/// <summary>
	/// OutputComponent��ǉ�
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(Args... args);
	void SetDeleteFlag();
	bool GetDeleteFlag() const;
	//���ׂĂ̗L���Ȏqcomponent��delete flag��t����
	void SetAllCompsFlag();
	//�qComponent�̓������ς݂̂��̂�o�^��������(Scene::ProcessPandingComps�p)
	void UnregisterInvalidChilds();
	//this_sh_���Z�b�g����(Scene::AddObject�����x�����Ăяo�����)
	void SetSharedPtr(std::shared_ptr<GameObject> obj);
	//this_sh_�����Z�b�g���A���̊֐��̌Ăяo������Ƀf�X�g���N�^���s(�����͌Ăяo�����`�F�b�N�p)
	void ResetSharedPtr(std::shared_ptr<GameObject> obj);
private:
	//comps_�̃A���P�[�^�Ƃ��ėp����
	struct CompsAllocator {
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
	//���Ƃ�������Ȃ�Amain�I����Ă��������������Ȃ��̂͂�����ƃL�����̂Ŏ����Ńv�[������čڂ�����
	std::list<ComponentHandle<Component>,
		boost::fast_pool_allocator<ComponentHandle<Component>,CompsAllocator>> comps_;
	boost::mutex comps_mutex_;
	bool delete_flag_;
	//�������w��shared_ptr
	std::shared_ptr<GameObject> this_sh_;
};

template<class T, class ...Args>
inline ComponentHandle<T> GameObject::AddUpdateComponent(Args ...args)
{
	auto handle = scene_->AddUpdateComponent<T>(this_sh_, args...);
	if (handle) {
		{
			boost::unique_lock<boost::mutex> lock(comps_mutex_);
			comps_.push_back(handle);
		}
		if (delete_flag_) {
			handle->SetDeleteFlag();
		}
	}
	return handle;
}

template<class T, class ...Args>
inline ComponentHandle<T> GameObject::AddOutputComponent(Args ...args)
{
	auto handle = scene_->AddOutputComponent<T>(this_sh_, args...);
	if (handle) {
		{
			boost::unique_lock<boost::mutex> lock(comps_mutex_);
			comps_.push_back(handle);
		}
		if (delete_flag_) {
			handle->SetDeleteFlag();
		}
	}
	return handle;
}
