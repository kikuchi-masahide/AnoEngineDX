#pragma once

class Game;
class Layer;

/// <summary>
/// �V�[����\���N���X
/// </summary>
class Scene {
public:
	Scene(boost::weak_ptr<Game> _game);
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
	/// ���C���[��������
	/// </summary>
	void AddLayer(boost::shared_ptr<Layer> _layer);
	const boost::weak_ptr<Game> mGame;
	bool GetDeleteFlag() const { return mDeleteFlag; };
	void SetDeleteFlag() { mDeleteFlag = true; };
	virtual ~Scene();
	boost::weak_ptr<Scene> GetWeakThis() const;
	void SetWeakThis(boost::shared_ptr<Scene> _this);
protected:
	/// <summary>
	/// �S���C���[��Update()���Ăяo��
	/// </summary>
	void UpdateLayers();
	/// <summary>
	/// �S���C���[��Output()���Ăяo��
	/// </summary>
	void OutputLayers();
	std::vector<boost::shared_ptr<Layer>> mLayers;
	std::vector<boost::shared_ptr<Layer>> mPendingLayers;
	//���C���[�X�V����true
	bool mIsLayerAddable;
	void InsertLayer(boost::shared_ptr<Layer> _layer);
	void DeleteLayers();
	bool mDeleteFlag;
	boost::weak_ptr<Scene> mThis;
};