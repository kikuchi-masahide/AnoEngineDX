#pragma once

#include "Math/Math.h"
#include "LayerHandle.h"

class Scene;
class DX12Resource;
class DX12DescriptorHeap;
class Game;
template<class T>
class LayerHandle;

/// <summary>
/// 特定の描画領域のペラポリゴンを担当するクラス
/// </summary>
class Layer {
public:
	Layer(Rect2 _rect, double _z, unsigned int _swap);
	/// <summary>
	/// Outputコンポーネント・Scene::UniqueOutputの後にz降順で呼び出す
	/// ペラポリゴンのバックバッファへの最終的な描画を行う．
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 専有座標が本当に変更されるのはDraw実行前
	/// </summary>
	void SetRect(Rect2 _rect);
	Rect2 GetRect();
	/// <summary>
	/// z座標が本当に変更されるのはDraw実行前
	/// </summary>
	void SetZ(double _z);
	double GetZ() const;
	//z座標の変更があったか否か
	bool HasZChanged();
	/// <summary>
	/// Sceneからのみ呼び出せる，Rect，zの変更フラッシュ用の関数
	/// </summary>
	void FlushZRectChange(Layer* _layer);
	bool GetDeleteFlag() const
	{
		return mDeleteFlag;
	}
	void SetDeleteFlag()
	{
		mDeleteFlag = true;
	}
	/// <summary>
	/// このレイヤーがペラポリゴンを貼り付けるスワップチェーンのIDを取得
	/// </summary>
	/// <returns></returns>
	unsigned int GetSwapchainID();
protected:
	virtual ~Layer();
	/// <summary>
	/// 自身を指すT型のLayerHandleを取得
	/// </summary>
	template<class T>
	LayerHandle<T> This()
	{
		return LayerHandle<T>((T*)this, &mHandles);
	}
private:
	friend Scene;
	//Sceneはデストラクタを呼ぶ直前にmDeleteCheckをtrueにする
	bool mDeleteCheck;
	//自分を指すハンドルの集合(void*にするのをやめたい)
	std::set<void*> mHandles;
	//&によるポインタ取得を禁止
	Layer* operator&() const noexcept;
	//Draw実行前の変更のストックとそのフラグ
	bool mWasZChanged;
	bool mWasRectChanged;
	double z;
	double mZAfter;
	//クライアント上の専有領域
	Rect2 mRect;
	Rect2 mRectAfter;
	unsigned int mSwapchainID;
	bool mDeleteFlag;
};