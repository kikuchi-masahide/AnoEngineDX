#pragma once

class Scene;
class GameObject;
class Game;

class Layer {
public:
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
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="">�����V�[��</param>
	/// <param name="">�N���C�A���g���W�ł̐�L�͈�</param>
	/// <param name="">�D��x(�傫����Α傫���قǐ�ɍX�V�o�͂��s����)</param>
	Layer(boost::weak_ptr<Scene> _scene, Rect2 _rect, int _order = 0);
	/// <summary>
	/// �I�u�W�F�N�g��ǉ�����
	/// </summary>
	void AddObject(boost::shared_ptr<GameObject> _obj);
	int GetUpdPriority() const;
	const boost::weak_ptr<Scene> mScene;
	/// <summary>
	/// �N���C�A���g���W�ł̐�L�͈͂̎擾
	/// </summary>
	Rect2 GetClientPosition() const;
	/// <summary>
	/// �N���C�A���g���W�ł̐�L�͈͂̕ύX
	/// </summary>
	void SetClientPosition(Rect2 _rect);
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Layer();
	void SetWeakThis(boost::shared_ptr<Layer> _this);
	boost::weak_ptr<Layer> GetWeakThis() const;
	boost::weak_ptr<Game> GetGame() const;
protected:
	/// <summary>
	/// �S�I�u�W�F�N�g��Update()���Ăяo��
	/// </summary>
	void UpdateObjects();
	/// <summary>
	/// �S�I�u�W�F�N�g��Output()���Ăяo��
	/// </summary>
	void OutputObjects();
	std::vector<boost::shared_ptr<GameObject>> mObjects;
	//�ۗ��I�u�W�F�N�g��vector
	std::vector <boost::shared_ptr<GameObject>> mPandingObjects;
	/// <summary>
	/// ���C���[�̍���A�E�����W
	/// </summary>
	Rect2 mRect;
	//�D��x(�傫�����ɍX�V�o��)
	int mUpdPriority;
	bool mIsObjAddable;
	void InsertObject(boost::shared_ptr<GameObject> _obj);
	bool mDeleteFlag;
	void DeleteObjects();
	boost::weak_ptr<Layer> mThis;
};