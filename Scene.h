#pragma once

class Game;
class Layer;

/// <summary>
/// シーンを表すクラス
/// </summary>
class Scene {
public:
	Scene(boost::weak_ptr<Game> _game);
	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();
	void CommonUpdate();
	virtual void UniqueUpdate();
	/// <summary>
	/// 出力関数
	/// </summary>
	void Output();
	void CommonOutput();
	virtual void UniqueOutput();
	/// <summary>
	/// レイヤーを加える
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
	/// 全レイヤーのUpdate()を呼び出す
	/// </summary>
	void UpdateLayers();
	/// <summary>
	/// 全レイヤーのOutput()を呼び出す
	/// </summary>
	void OutputLayers();
	std::vector<boost::shared_ptr<Layer>> mLayers;
	std::vector<boost::shared_ptr<Layer>> mPendingLayers;
	//レイヤー更新中はtrue
	bool mIsLayerAddable;
	void InsertLayer(boost::shared_ptr<Layer> _layer);
	void DeleteLayers();
	bool mDeleteFlag;
	boost::weak_ptr<Scene> mThis;
};