#pragma once
#include "Component.h"
#include "ComponentHandle.h"
#include "LayerHandle.h"
#include "Game.h"

class Layer2D;

/// <summary>
/// Layer2Dに結び付けられた出力コンポーネント
/// </summary>
class Layer2DComponent :public Component {
public:
	/// <param name="_layer">所属Layer2D</param>
	/// <param name="_texture">描画するテクスチャ</param>
	/// <param name="_z">z座標 大きいほど奥側</param>
	/// <param name="_rect">レイヤー上の描画範囲(1px単位)</param>
	Layer2DComponent(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, LayerHandle<Layer2D> _layer, unsigned int _texture, double _z, Rect2 _rect);
	virtual void Update();
	~Layer2DComponent();
	/// <summary>
	/// Layer2Dの画面出力時，レンダーターゲットをペラポリゴンに指定した状態で呼び出される．
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// Layer左上を中心とした1px単位の座標
	/// 1 3
	/// 0 2
	/// </summary>
	Vector3 mVerts[4];
	/// <summary>
	/// UV座標
	/// 1 3
	/// 0 2
	/// </summary>
	Vector2 mUV[4];
	double z;
protected:
	LayerHandle<Layer2D> mLayer;
	unsigned int mTextureID;
};