//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "ConfigEnums.h"

namespace DX12 {
	/// <summary>
	/// DescriptorTableタイプのRootParameterに含まれるDescriptorRange
	/// </summary>
	struct DescriptorRange {
	public:
		//捕捉するdescriptorの種類
		int num_descriptors_;
		//捕捉するdescriptorの種類
		DescriptorRangeType range_type_;
		//シェーダレジスタの何番目から占有するか
		//(例えばmBaseShaderRegister = 2,mNumDescriptors = 4,mType = CBVならば，
		//このディスクリプタレンジはb2からb5を占有する)
		int base_shader_register_;
		//ディスクリプタヒープの何番目から占有するか
		//(例えばmBaseHeapRegister = 2,mNumDescriptors = 4ならば，
		//ディスクリプタヒープの2から5番目を占有する)
		//-1ならば、D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND、つまり前のRangeの直後と解釈する
		int base_heap_register_;
		DescriptorRange(int num_desc, DescriptorRangeType range_type, int base_shader_register,
			int base_heap_register = -1)
			:num_descriptors_(num_desc),range_type_(range_type),base_shader_register_(base_shader_register),
			base_heap_register_(base_heap_register)
		{}
	};
}