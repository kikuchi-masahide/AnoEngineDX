#pragma once
#include "Scene.h"
#include "Component1.h"
#include "Component2.h"
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "ComponentHandle.h"

class Scene1 :public Scene
{
public:
	Scene1(Game* _game)
		:Scene(_game)
	{
		Log::OutputTrivial("Scene1 Initializer");
		auto handle = AddObject(GetVector2(0,0),1.0,0.0);
		auto handlec11 = handle->AddOutputComponent<Component1>(10,0);
		auto handlec12 = handle->AddOutputComponent<Component1>(60,1);
		handle->AddUpdateComponent<Component2>(60, 0,handle);
		handle->AddUpdateComponent<Component2>(20, 1,handle);
		Vector3 vertices[] = {
			GetVector3(-0.4f,-0.7f,0.0f),
			GetVector3(-0.4f,0.7f,0.0f),
			GetVector3(0.4f,-0.7f,0.0f),
			GetVector3(0.4f,0.7f,0.0f)
		};
		mVS = mGame.mdx12.LoadShader(L"BasicVertexShader.hlsl", DX12Config::ShaderType::VERTEX);
		mPS = mGame.mdx12.LoadShader(L"BasicPixelShader.hlsl", DX12Config::ShaderType::PIXEL);
		mResource = _game->mdx12.CreateVertexBuffer(sizeof(float) * 3 * 4);
		float* resmap = (float*)_game->mdx12.Map(mResource);
		for (unsigned int n = 0; n < 4; n++) {
			resmap[3 * n] = boost::qvm::X(vertices[n]);
			resmap[3 * n + 1] = boost::qvm::Y(vertices[n]);
			resmap[3 * n + 2] = boost::qvm::Z(vertices[n]);
		}
		_game->mdx12.Unmap(mResource);
		mLayout.push_back(DX12VertexLayoutUnit(
			"POSITION", DX12Config::VertexLayoutFormat::R32G32B32_FLOAT, 0,
			DX12Config::VertexLayoutInputClassification::INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1
		));
		mRootSignature = _game->mdx12.CreateRootSignature();
		mPipeline = _game->mdx12.CreateGraphicsPipeline(
			mVS, mPS, mLayout, DX12Config::PrimitiveTopologyType::TRIANGLESTRIP, 1, mRootSignature
		);
		unsigned int indeces[] = {
			0,1,2,
			2,1,3
		};
		mIndeces = _game->mdx12.CreateIndexBuffer(6);
		unsigned int* indresmap = (unsigned int*)_game->mdx12.Map(mIndeces);
		for (unsigned int n = 0; n < 6; n++) {
			indresmap[n] = indeces[n];
		}
		_game->mdx12.Unmap(mIndeces);
	}
	void UniqueOutput()
	{
		Log::OutputTrivial("Scene1 UniqueOutput");
		mGame.mdx12.SetGraphicsPipeline(mPipeline);
		mGame.mdx12.SetRootSignature(mRootSignature);
		mGame.mdx12.SetPrimitiveTopology(DX12Config::PrimitiveTopologyType::TRIANGLESTRIP);
		mGame.mdx12.SetVertexBuffers(mResource, 0, sizeof(float) * 12, sizeof(float) * 3);
		mGame.mdx12.SetIndexBuffers(mIndeces, 6);
		mGame.mdx12.SetViewports(1024, 768, 0, 0, 1.0f, 0.0f);
		mGame.mdx12.SetScissorrect(0, 768, 0, 1024);
		mGame.mdx12.DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
	void UniqueUpdate()
	{
		Log::OutputTrivial("Scene1 UniqueUpdate");
	}
	~Scene1()
	{
		Log::OutputTrivial("Scene1 destructor");
	}
private:
	boost::shared_ptr<DX12Resource> mResource;
	boost::shared_ptr<DX12Resource> mIndeces;
	boost::shared_ptr<DX12ShaderObject> mVS, mPS;
	boost::shared_ptr<DX12GraphicsPipeline> mPipeline;
	boost::shared_ptr<DX12RootSignature> mRootSignature;
	DX12VertexLayout mLayout;
};