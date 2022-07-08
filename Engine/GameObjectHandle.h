//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class GameObject;

/// <summary>
/// GameObject���������Ɏg���n���h�� Scene::AddChild�̕Ԃ�l
/// �|�C���^�̂悤��->���Z�q�Ń����o�ɃA�N�Z�X�\
/// </summary>
class GameObjectHandle {
public:
	GameObjectHandle(const GameObjectHandle& handle);
	/// <summary>
	/// Scene��AddChild���ł̂݌Ăяo�����R���X�g���N�^
	/// </summary>
	GameObjectHandle(GameObject* obj, std::unordered_set<GameObjectHandle*>* set);
	GameObjectHandle();
	GameObjectHandle& operator=(const GameObjectHandle& handle);
	~GameObjectHandle();
	bool IsValid() const;
	GameObject* operator->() const noexcept;
	//���̃I�u�W�F�N�g�n���h�����N���A���Cnull�n���h���ɂ���
	void Reset();
private:
	//���̃n���h�����w���I�u�W�F�N�g
	GameObject* object_;
	//mObject���w���n���h����std::set�̃|�C���^
	std::unordered_set<GameObjectHandle*>* handles_set_;
};