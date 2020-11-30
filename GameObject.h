#pragma once

#include "Scene.h"

class Component;
class Game;
class GameObjectHandle;
class ComponentHandle;

template<class T>
concept ComponentInheritenc = requires (T & x) {

}

/// <summary>
/// �V�[���Ɋ܂܂��I�u�W�F�N�g��\���N���X
/// </summary>
class GameObject final {
public:
	/// <summary>
	/// Scene::AddChild����Ăяo�����R���X�g���N�^
	/// </summary>
	GameObject(Scene* _scene, boost::shared_ptr<std::set<GameObjectHandle*>> _hset, Vector2 _pos, double _scale, double _angle);
	Vector2 GetPosition() const;
	Vector2 SetPosition(Vector2 _pos);
	double GetScale() const;
	double SetScale(double _sc);
	double GetRotation() const;
	double SetRotation(double _ro);
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	~GameObject();
	//�e���v���[�g�̍�����֐��̒�`��.h��u���Ȃ��Ƃ����Ȃ��͉̂��̂��ꉞ���ׂ�
	template<class T,class... Args>
	ComponentHandle AddUpdateComponent(Args... _args) {
		//���̃R���|�[�l���g���w���n���h���̏W��
		boost::shared_ptr<std::set<ComponentHandle*>> comphsetp(new std::set<ComponentHandle*>());
		//�R���|�[�l���g���g
		boost::shared_ptr<Component> compp(new T(this, comphsetp, _args...));
		mUpdateComponents.push_back(compp);
		//�Ԃ��n���h��
		ComponentHandle comph(compp.get(), comphsetp);
		//�V�[���ɒǉ�
		mScene->AddUpdateComponent(this,comph);
		return comph;
	};
	template<class T, class... Args>
	ComponentHandle AddOutputComponent(Args... _args) {
		//���̃R���|�[�l���g���w���n���h���̏W��
		boost::shared_ptr<std::set<ComponentHandle*>> comphsetp(new std::set<ComponentHandle*>());
		//�R���|�[�l���g���g
		boost::shared_ptr<Component> compp(new T(this, comphsetp, _args...));
		mOutputComponents.push_back(compp);
		//�Ԃ��n���h��
		ComponentHandle comph(compp.get(), comphsetp);
		//�V�[���ɒǉ�
		mScene->AddOutputComponent(this,comph);
		return comph;
	};
	//�t���O�������Ă���R���|�[�l���g���폜
	void DeleteFlagedComponents(Scene* _scene);
	Scene& GetScene() const { return *mScene; };
	Game& GetGame() const;
private:
	Scene* mScene;
	/// <summary>
	/// �I�u�W�F�N�g�̒��S���W
	/// </summary>
	Vector2 mPosition;
	double mScale;
	/// <summary>
	/// ��]�p�x(���v���A�x��)
	/// </summary>
	double mRotation;
	//���̃I�u�W�F�N�g�̎��X�V�E�o�̓R���|�[�l���g
	std::list<boost::shared_ptr<Component>> mUpdateComponents;
	std::list<boost::shared_ptr<Component>> mOutputComponents;
	//���̃I�u�W�F�N�g���w���n���h����set
	boost::shared_ptr<std::set<GameObjectHandle*>> mHandles;
	bool mDeleteFlag;
	GameObject* operator&() const noexcept;
};