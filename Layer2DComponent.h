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
	/// <summary>
	/// Layer2D上のz座標．mVertsのz座標は描画順序に関与しないことに注意
	/// </summary>
	double z;
	/// <summary>
	/// 描画領域の平行移動
	/// </summary>
	void TranslationRect(Vector3 _vect);
	/// <summary>
	/// 描画領域を，領域の中心を中心として各軸方向に拡大
	/// </summary>
	/// <param name="_x">x軸方向の拡大率</param>
	/// <param name="_y">y軸方向の拡大率</param>
	/// <param name="_z">z軸方向の拡大率</param>
	void ExpandRect(double _x, double _y, double _z);
	/// <summary>
	/// 描画領域を，_centerを中心として各軸方向に拡大
	/// </summary>
	/// <param name="_x">x軸方向の拡大率</param>
	/// <param name="_y">y軸方向の拡大率</param>
	/// <param name="_z">z軸方向の拡大率</param>
	/// <param name="_center">拡大中心</param>
	void ExpandRect(double _x, double _y, double _z, Vector3 _center);
	/// <summary>
	/// 描画領域を，その中心を中心として_vect方向に_deg回転
	/// </summary>
	void RotateRect(Vector3 _vect, double _deg);
	/// <summary>
	/// 描画領域を，_centerを中心として_vect方向に_deg回転
	/// </summary>
	void RotateRect(Vector3 _vect, double _deg, Vector3 _center);
	/// <summary>
	/// 描画領域にこの行列を適用
	/// </summary>
	void ApplyMatrix(Matrix4x4 _mat);
protected:
	LayerHandle<Layer2D> mLayer;
	unsigned int mTextureID;
};