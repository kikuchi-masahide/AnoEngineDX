#pragma once

class Layer;
class Component;
class Game;
class Scene;

/// <summary>
/// ���C���[�Ɋ܂܂��I�u�W�F�N�g��\���N���X
/// </summary>
class GameObject {
public:
	GameObject(boost::weak_ptr<Layer> _layer, Vector2 _pos, double _scale, double _angle, int _prio);
	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update();
	void CommonUpdate();
	virtual void UniqueUpdate();
	/// <summary>
	/// �o�͊֐�
	/// </summary>
	void Output();
	void CommonOutput();
	virtual void UniqueOutput();
	/// <summary>
	/// �X�V�R���|�[�l���g��ǉ�����
	/// </summary>
	void AddComponent(boost::shared_ptr<Component> _comp);
	/// <summary>
	/// �o�̓R���|�[�l���g��ǉ�����
	/// </summary>
	void AddOutputComponent(boost::shared_ptr<Component> _comp);
	Vector2 GetPosition() const;
	Vector2 SetPosition(Vector2 _pos);
	double GetScale() const;
	double SetScale(double _sc);
	double GetRotation() const;
	double SetRotation(double _ro);
	/// <summary>
	/// �����̏������郌�C���[
	/// </summary>
	const boost::weak_ptr<Layer> mLayer;
	int GetUpdPriority() const;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~GameObject();
	boost::weak_ptr<Scene> GetScene() const;
	boost::weak_ptr<Game> GetGame() const;
	boost::weak_ptr<GameObject> GetWeakThis() const;
	void SetWeakThis(boost::shared_ptr<GameObject> _this);
protected:
	/// <summary>
	/// �I�u�W�F�N�g�̒��S���W
	/// </summary>
	Vector2 mPosition;
	double mScale;
	/// <summary>
	/// ��]�p�x(���v���A�x��)
	/// </summary>
	double mRotation;
	/// <summary>
	/// �X�V�R���|�[�l���g
	/// </summary>
	std::vector<boost::shared_ptr<Component>> mUpdateComponents;
	/// <summary>
	/// �o�̓R���|�[�l���g
	/// </summary>
	std::vector<boost::shared_ptr<Component>> mOutputComponents;
	void UpdateUpdComponents();
	void UpdateOutputComponents();
	void DeleteUpdComponents();
	void DeleteOutputComponents();
	//�����C���[�ɂ����ėD��x���傫���قǐ�ɍX�V�����
	int mUpdPriority;
	bool mDeleteFlag;
	boost::weak_ptr<GameObject> mThis;
};