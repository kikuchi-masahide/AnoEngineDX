#pragma once

class Layer;
class Component;
class Game;
class Scene;
class GameObjectHandle;
class ComponentHandle;

/// <summary>
/// ���C���[�Ɋ܂܂��I�u�W�F�N�g��\���N���X
/// </summary>
class GameObject final {
public:
	/// <summary>
	/// Scene::AddChild����Ăяo�����R���X�g���N�^
	/// </summary>
	GameObject(Scene* _scene, boost::shared_ptr<std::set<GameObjectHandle*>> _hset, Vector2 _pos, double _scale, double _angle);
	///// <summary>
	///// �X�V�֐�
	///// </summary>
	//void Update();
	///// <summary>
	///// �o�͊֐�
	///// </summary>
	//void Output();
	Vector2 GetPosition() const;
	Vector2 SetPosition(Vector2 _pos);
	double GetScale() const;
	double SetScale(double _sc);
	double GetRotation() const;
	double SetRotation(double _ro);
	Scene& mScene;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	~GameObject();
	template<class T,class... Args>
	ComponentHandle AddUpdateComponent(Args... _args);
	template<class T, class... Args>
	ComponentHandle AddOutputComponent(Args... _args);
private:
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
};