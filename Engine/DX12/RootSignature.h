//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"
#include "DescriptorRange.h"

namespace DX12 {
	//HACK:サンプラーを可変にできるようにするべきか?
	/// <summary>
	/// ルートシグネチャのクラス
	/// </summary>
	class RootSignature final:public boost::noncopyable
	{
	public:
		/// <param name="root_param_num">RootParameterの数(std::vectorの初期reserve数)</param>
		RootSignature(int root_param_num);
		~RootSignature();
		//現在の内容でシリアライズする(シリアライズ後ルートパラメタやサンプラーは変更できない)
		void Serialize(ComPtr<ID3D12Device> device);
		void SetRootSignature(ComPtr<ID3D12GraphicsCommandList> list);
		//GraphicsPipeline用
		ID3D12RootSignature* GetRawPtr();
		//DescriptorTableタイプのRootParameterを追加する
		void AddRootParameterAsDescriptorTable(std::vector<DescriptorRange>& ranges, RootParameterShaderVisibility vis);
	private:
		ComPtr<ID3D12RootSignature> root_signature_;
		//シリアライズ済ならばtrue
		bool serialized_;
		std::vector<CD3DX12_ROOT_PARAMETER> root_params_;
		//root_params_の設定で使うDescriptorRangeの配列の配列
		//後でdeleteする
		std::vector<D3D12_DESCRIPTOR_RANGE*> ranges_;
	};
}

