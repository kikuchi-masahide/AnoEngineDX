//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "Log.h"

class Component;

//Component���p�������N���X�̃|�C���^����Ɏg���n���h��
template<class T>
class ComponentHandle final {
public:
	//�R�s�[�R���X�g���N�^
	ComponentHandle(const ComponentHandle<T>& handle)
		:comp_(handle.comp_), handles_set_(handle.handles_set_)
	{
		if (handles_set_ != nullptr) {
			handles_set_->insert((void*)this);
		}
	}
	//GameObject��AddChild�ł̂ݎg����R���X�g���N�^
	ComponentHandle(T* obj, std::unordered_set<void*>* set)
		:comp_(obj), handles_set_(set)
	{
		if (comp_ != nullptr) {
			handles_set_->insert((void*)this);
		}
	}
	ComponentHandle()
		: comp_(nullptr), handles_set_(nullptr)
	{}
	ComponentHandle(const T* comp)
		:comp_(comp), handles_set_(comp->mHandles)
	{
		handles_set_->insert((void*)this);
	}
	ComponentHandle<T>& operator=(const ComponentHandle<T>& h) {
		Reset();
		if (h.comp_ != nullptr) {
			comp_ = h.comp_;
			handles_set_ = h.handles_set_;
			handles_set_->insert((void*)this);
		}
		return *this;
	}
	~ComponentHandle()
	{
		Reset();
	}
	T* operator->() const noexcept
	{
		//�_���O�����O�|�C���^�Ȃ��assert���o��
		if (comp_ == nullptr) {
			Log::OutputCritical("Dungling Pointer");
		}
		return comp_;
	}
	bool IsValid() const
	{
		return (comp_ != nullptr);
	}
	void Reset()
	{
		if (comp_ != nullptr) {
			handles_set_->erase(static_cast<void*>(this));
			handles_set_ = nullptr;
			comp_ = nullptr;
		}
	}
	template<class U>
	explicit operator ComponentHandle<U>() const noexcept {
		//�p���֌W�̃`�F�b�N
		U* self;
		try {
			self = dynamic_cast<U*>(comp_);
		}
		catch (std::bad_cast& b) {
			Log::OutputCritical("bad_cast\n");
		}
		return ComponentHandle<U>(self, handles_set_);
	}
	//bool�^�ŃL���X�g�����S���ꉻ �_���O�����O�n���h���ł��邩�ۂ�
	explicit operator bool() const noexcept
	{
		return (comp_ != nullptr);
	}
	bool operator!() const noexcept
	{
		return !static_cast<bool>(*this);
	}
	//�w���Ă���R���|�[�l���g�������Ȃ�Γ�����
	bool operator==(const ComponentHandle<T>& handle) const;
	bool operator<(const ComponentHandle<T>& handle) const
	{
		return comp_ < handle.comp_;
	}
private:
	//�{���͂�肽���Ȃ�
	template<class U>
	friend bool operator<(const ComponentHandle<U>& handle1, const ComponentHandle<U>& handle2);
	//�n���h�����w���R���|�[�l���g
	T* comp_;
	//mComp���w���n���h����set�̃|�C���^(void*���g���̂͂ł���΂�߂���)
	std::unordered_set<void*>* handles_set_;
};

template<class T>
inline bool ComponentHandle<T>::operator==(const ComponentHandle<T>& handle) const
{
	return (comp_ == handle.comp_);
}

template<class T>
//std::set���ȂǂŎg���ۂ̗\��
bool operator<(const ComponentHandle<T>& handle1, const ComponentHandle<T>& handle2)
{
	return (handle1.comp_ < handle2.comp_);
}