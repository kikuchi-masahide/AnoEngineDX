#pragma once
#include "Layer.h"
#include "Game.h"
#include "ComponentHandle.h"

class Layer2DComponent;

/// <summary>
/// 画家のアルゴリズムによる2D描画用のレイヤー
/// </summary>
class Layer2D :public Layer {
public:
	Layer2D(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap);
	~Layer2D();
	virtual void Draw();
	/// <summary>
	/// このレイヤーに描画を行うコンポーネントを追加する．
	/// </summary>
	void InsertComponent(ComponentHandle<Layer2DComponent> _comp);
private:
	boost::shared_ptr<DX12Resource> mVertResource;
	boost::shared_ptr<DX12Resource> mIndexResource;
	boost::shared_ptr<DX12GraphicsPipeline> mPipeline;
	boost::shared_ptr<DX12RootSignature> mRootSignature;
	boost::shared_ptr<DX12Resource> mTexReadResource;
	boost::shared_ptr<DX12Resource> mBackbuffer;
	boost::shared_ptr<DX12DescriptorHeap> mRTV;
	boost::shared_ptr<DX12DescriptorHeap> mSRV;
	DX12VertexLayout mVertLayout;
	struct VertexLayout {
	public:
		MatVec::Vector3 mPos;
		MatVec::Vector2 mUV;
	};
	/// <summary>
	/// コンストラクタで呼ぶDirect3D12まわりの初期化関数
	/// </summary>
	void GraphicalInit();
	//Layer2DCoimponentをz降順で呼びだす(左手系!)
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle<Layer2DComponent>& left, const ComponentHandle<Layer2DComponent>& right) const;
	};
	std::set<ComponentHandle<Layer2DComponent>,ComponentHandleCompare> mComps;
};