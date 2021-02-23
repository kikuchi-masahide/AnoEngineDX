#pragma once

class Scene;
class DX12Resource;
class DX12DescriptorHeap;
class Game;

class Layer {
public:
	Layer(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap);
	virtual ~Layer();
	//Outputコンポーネント・Scene::UniqueOutputの後にz降順で呼び出す
	//ペラポリゴンのバックバッファへの最終的な描画を行う．
	virtual void Draw() = 0;
	//専有座標が本当に変更されるのはDraw実行前
	void SetRect(Rect2 _rect);
	Rect2 GetRect();
	//z座標が本当に変更されるのはDraw実行前
	void SetZ(double _z);
	double GetZ();
	//親シーン
	Scene& mScene;
	Game& GetGame() const;
	//Sceneからのみ呼び出せる，Rect，zの変更フラッシュ用の関数
	void FlushZRectChange(Layer* _layer);
private:
	double z;
	//クライアント上の専有領域
	Rect2 mRect;
	//自分を指すハンドルの集合のポインタ(void*を使うのは何というかやめたい)
	boost::shared_ptr<std::set<void*>> mHandles;
	//&によるポインタ取得を禁止
	Layer* operator&() const noexcept;
	//Draw実行前の変更のストックとそのフラグ
	bool mWasZChanged;
	double mZAfter;
	bool mWasRectChanged;
	Rect2 mRectAfter;
	unsigned int mSwapchainID;
};