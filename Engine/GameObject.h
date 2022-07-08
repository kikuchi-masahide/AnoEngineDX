#pragma once

#include "Component.h"
#include "GameObjectHandle.h"
#include "Math/Math.h"

class Scene;
class Game;
template<class T>
class ComponentHandle;

/// <summary>
/// �V�[���Ɋ܂܂��I�u�W�F�N�g��\���N���X
/// </summary>
class GameObject final {
public:
	Scene* const scene_;
	/// <summary>
	/// Scene::AddChild����Ăяo�����R���X�g���N�^
	/// </summary>
	GameObject(Scene* const scene, MatVec::Vector2 pos, double scale, double angle);
	/// <summary>
	/// ���̃I�u�W�F�N�g��T�^��UpdateComponent��ǉ�(_args�̓R���X�g���N�^�ɓn�������ŁC������mObj������������)
	/// concept����T�̐���������X(�ÓI��)�]��������
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddUpdateComponent(Args... args) {
		//�R���|�[�l���g���g
		T* comp = new T(This(), args...);
		update_components_.push_back(comp);
		ComponentHandle<T> handle = comp->This<T>();
		//�V�[���ɒǉ�
		AddUpdateComponentToScene(static_cast<ComponentHandle<Component>>(handle));
		return handle;
	}
	/// <summary>
	/// ���̃I�u�W�F�N�g��T�^��OutputComponent��ǉ�(_args�̓R���X�g���N�^�ɓn�������ŁC������mObj������������)
	/// concept����T�̐���������X(�ÓI��)�]��������
	/// </summary>
	template<class T, class... Args>
	ComponentHandle<T> AddOutputComponent(Args... args) {
		//�R���|�[�l���g���g
		T* comp = new T(This(), args...);
		output_components_.push_back(comp);
		ComponentHandle<T> handle = comp->This<T>();
		//�V�[���ɒǉ�
		AddOutputComponentToScene(static_cast<ComponentHandle<Component>>(handle));
		return handle;
	};
	MatVec::Vector2 GetPosition() const;
	MatVec::Vector2 SetPosition(MatVec::Vector2 pos);
	double GetScale() const;
	double SetScale(double sc);
	double GetRotation() const;
	double SetRotation(double ro);
	bool GetDeleteFlag() const;
	void SetDeleteFlag();
	Game& GetGame();
private:
	friend class Scene;
	~GameObject();
	//AddUpdateComponent���ŌĂ΂��
	//#include�̕�������v�����邽�ߕʊ֐��ɕ���
	void AddUpdateComponentToScene(ComponentHandle<Component> handle);
	void AddOutputComponentToScene(ComponentHandle<Component> handle);
	/// <summary>
	/// Scene::AddObject�ŌĂяo�����C���g���w���n���h����Ԃ��֐�
	/// </summary>
	GameObjectHandle This();
	//���̃R���|�[�l���g��delete���f�X�g���N�^���Ă�
	//(���Ƃ��Ƃ��̃|�C���^�������Ă���list�����erase�͂��Ȃ�)
	void DeleteComponent(Component* component);
	//�t���O�������Ă���R���|�[�l���g���폜
	void DeleteFlagedComponents();
	/// <summary>
	/// �I�u�W�F�N�g�̒��S���W
	/// </summary>
	MatVec::Vector2 position_;
	double scale_;
	/// <summary>
	/// ��]�p�x(���v���A�x��)
	/// </summary>
	double rotation_;
	//���̃I�u�W�F�N�g�̎��X�V�E�o�̓R���|�[�l���g
	std::list<Component*> update_components_;
	std::list<Component*> output_components_;
	//���̃I�u�W�F�N�g���w���n���h����set
	std::unordered_set<GameObjectHandle*> handles_;
	bool delete_flag_;
	GameObject* operator&() const noexcept;
};