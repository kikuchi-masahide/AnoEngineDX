#pragma once
#include "Layer.h"
#include "Game.h"
#include "ComponentHandle.h"

class Layer2DComponent;

/// <summary>
/// ��Ƃ̃A���S���Y���ɂ��2D�`��p�̃��C���[
/// </summary>
class Layer2D :public Layer {
public:
	Layer2D(Scene* _scene, boost::shared_ptr<std::set<void*>> _hset, Rect2 _rect, double _z, unsigned int _swap);
	~Layer2D();
	virtual void Draw();
	/// <summary>
	/// ���̃��C���[�ɕ`����s���R���|�[�l���g��ǉ�����D
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
	/// �R���X�g���N�^�ŌĂ�Direct3D12�܂��̏������֐�
	/// </summary>
	void GraphicalInit();
	//Layer2DCoimponent��z�~���ŌĂт���(����n!)
	class ComponentHandleCompare {
	public:
		bool operator()(const ComponentHandle<Layer2DComponent>& left, const ComponentHandle<Layer2DComponent>& right) const;
	};
	std::set<ComponentHandle<Layer2DComponent>,ComponentHandleCompare> mComps;
};