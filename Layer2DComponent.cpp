#include "Layer2DComponent.h"
#include "Layer2D.h"

Layer2DComponent::Layer2DComponent(GameObject* _owner, boost::shared_ptr<std::set<void*>> _hset, LayerHandle<Layer2D> _layer, unsigned int _texture, double _z, Rect2 _rect)
	:Component(_owner, _hset, 0),mLayer(_layer),mTextureID(_texture),z(_z)
{
	auto lx = boost::qvm::X(_rect.GetLD());
	auto by = boost::qvm::Y(_rect.GetLD());
	auto rx = boost::qvm::X(_rect.GetRU());
	auto uy = boost::qvm::Y(_rect.GetRU());
	mVerts[0] = GetVector3(lx, by, 0);
	mVerts[1] = GetVector3(lx, uy, 0);
	mVerts[2] = GetVector3(rx, by, 0);
	mVerts[3] = GetVector3(rx, uy, 0);
	mUV[0] = GetVector2(0, 1);
	mUV[1] = GetVector2(0, 0);
	mUV[2] = GetVector2(1, 1);
	mUV[3] = GetVector2(1, 0);
}

void Layer2DComponent::Update()
{
	mLayer->InsertComponent(This<Layer2DComponent>());
}

Layer2DComponent::~Layer2DComponent()
{
}

void Layer2DComponent::Draw()
{
	static boost::shared_ptr<DX12Resource> mIndexResource = nullptr;
	static boost::shared_ptr<DX12ShaderObject> mVS, mPS = nullptr;
	static boost::shared_ptr<DX12GraphicsPipeline> mPipeline = nullptr;
	static boost::shared_ptr<DX12RootSignature> mRootSignature = nullptr;
	static boost::shared_ptr<DX12Resource> mTexReadResource = nullptr;
	static DX12VertexLayout mVertLayout;
	static boost::shared_ptr<DX12Resource> mVertResource = nullptr;
	//初期化
	if (!mIndexResource)
	{
		mIndexResource = GetGame().mdx12.CreateIndexBuffer(6);
		unsigned int* mIndMap = (unsigned int*)GetGame().mdx12.Map(mIndexResource);
		mIndMap[0] = 0;
		mIndMap[1] = 1;
		mIndMap[2] = 2;
		mIndMap[3] = 2;
		mIndMap[4] = 1;
		mIndMap[5] = 3;
		GetGame().mdx12.Unmap(mIndexResource);
		auto mVS = GetGame().mShaderManager.GetDX12ShaderObject(0);
		auto mPS = GetGame().mShaderManager.GetDX12ShaderObject(1);
		mVertLayout.push_back(DX12VertexLayoutUnit(
			"POSITION", DX12Config::VertexLayoutFormat::R32G32B32_FLOAT, 0,
			DX12Config::VertexLayoutInputClassification::INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		));
		mVertLayout.push_back(DX12VertexLayoutUnit(
			"TEXCOORD", DX12Config::VertexLayoutFormat::R32G32_FLOAT, 0,
			DX12Config::VertexLayoutInputClassification::INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		));
		DX12DescriptorRange range1(1, DX12Config::DescriptorRangeType::SRV, 0);
		DX12RootParameter rootparam;
		rootparam.mShaderVisibility = DX12Config::RootParameterShaderVisibility::ALL;
		rootparam.mDescRanges.push_back(range1);
		mRootSignature = GetGame().mdx12.CreateRootSignature(rootparam);
		mPipeline = GetGame().mdx12.CreateGraphicsPipeline(
			mVS, mPS, mVertLayout, DX12Config::PrimitiveTopologyType::TRIANGLE, 1, mRootSignature
		);
		mVertResource = GetGame().mdx12.CreateVertexBuffer(sizeof(float) * 5 * 4);
	}
	//頂点情報を毎回入れなおす
	float* map = (float*)GetGame().mdx12.Map(mVertResource);
	auto rect = mLayer->GetRect();
	auto width = rect.GetWidth();
	auto height = rect.GetHeight();
	for (int n = 0; n < 5; n++) {
		auto x = boost::qvm::X(mVerts[n]);
		x = 2 * x / width - 1;
		auto y = boost::qvm::Y(mVerts[n]);;
		y = 2 * y / height - 1;
		map[5 * n] = x;
		map[5 * n + 1] = y;
		map[5 * n + 2] = 0;
		map[5 * n + 3] = boost::qvm::X(mUV[n]);
		map[5 * n + 4] = boost::qvm::Y(mUV[n]);
	}
	GetGame().mdx12.Unmap(mVertResource);
	GetGame().mdx12.SetGraphicsPipeline(mPipeline);
	GetGame().mdx12.SetRootSignature(mRootSignature);
	auto descheap = GetGame().mTexManager.GetDX12DescriptorHeap(mTextureID).first;
	auto descid = GetGame().mTexManager.GetDX12DescriptorHeap(mTextureID).second;
	GetGame().mdx12.SetDescriptorHeap(descheap);
	GetGame().mdx12.SetGraphicsRootDescriptorTable(0, descheap, descid);
	GetGame().mdx12.SetPrimitiveTopology(DX12Config::PrimitiveTopology::TRIANGLESTRIP);
	GetGame().mdx12.SetVertexBuffers(mVertResource, 0, sizeof(float) * 5 * 4, sizeof(float) * 5);
	GetGame().mdx12.SetIndexBuffers(mIndexResource, 6);
	GetGame().mdx12.SetViewports(width, height, 0, 0, 1.0f, 0.0f);
	GetGame().mdx12.SetScissorrect(0, height, 0, width);
	GetGame().mdx12.DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Layer2DComponent::TranslationRect(Vector3 _vect)
{
	for (unsigned int n = 0; n < 4; n++)
	{
		mVerts[n] += _vect;
	}
}

void Layer2DComponent::ExpandRect(double _x, double _y, double _z)
{
	Vector3 center = (mVerts[0] + mVerts[1] + mVerts[2] + mVerts[3]) / 4;
	auto exp = GetVector3(_x, _y, _z);
	for (unsigned int n = 0; n < 4; n++)
	{
		mVerts[n] -= center;
		mVerts[n] = boost::qvm::diag_mat(exp) * mVerts[n];
		mVerts[n] += center;
	}
}

void Layer2DComponent::ExpandRect(double _x, double _y, double _z, Vector3 _center)
{
	auto exp = GetVector3(_x, _y, _z);
	for (unsigned int n = 0; n < 4; n++)
	{
		mVerts[n] -= _center;
		mVerts[n] = boost::qvm::diag_mat(exp) * mVerts[n];
		mVerts[n] += _center;
	}
}

void Layer2DComponent::RotateRect(Vector3 _vect, double _deg)
{
	auto center = (mVerts[0] + mVerts[1] + mVerts[2] + mVerts[3]) / 4;
	//_vectが0ベクトルで正規化出来ない場合
	try {
		boost::qvm::normalize(_vect);
	}
	catch(...){
		BOOST_ASSERT_MSG(0, "rotate axis must not be 0");
	}
	auto const quat = boost::qvm::rot_quat(_vect, _deg);
	auto const mat = boost::qvm::convert_to<Matrix4x4>(quat);
	for (unsigned int n = 0; n < 4; n++)
	{
		mVerts[n] -= center;
		Vector4 vec4 = boost::qvm::XYZ1(mVerts[n]);
		vec4 = mat * vec4;
		mVerts[n] = boost::qvm::XYZ(vec4) + center;
	}
}

void Layer2DComponent::RotateRect(Vector3 _vect, double _deg, Vector3 _center)
{
	//_vectが0ベクトルで正規化出来ない場合
	try {
		boost::qvm::normalize(_vect);
	}
	catch (...) {
		BOOST_ASSERT_MSG(0, "rotate axis must not be 0");
	}
	auto const quat = boost::qvm::rot_quat(_vect, _deg);
	auto const mat = boost::qvm::convert_to<Matrix4x4>(quat);
	for (unsigned int n = 0; n < 4; n++)
	{
		mVerts[n] -= _center;
		Vector4 vec4 = boost::qvm::XYZ1(mVerts[n]);
		vec4 = mat * vec4;
		mVerts[n] = boost::qvm::XYZ(vec4) + _center;
	}
}

void Layer2DComponent::ApplyMatrix(Matrix4x4 _mat)
{
	for (unsigned int n = 0; n < 4; n++)
	{
		Vector4 vec = boost::qvm::XYZ1(mVerts[n]);
		vec = _mat * vec;
		mVerts[n] = boost::qvm::XYZ(vec);
	}
}
