//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12Include.h"
#include "ConfigEnums.h"

namespace DX12 {
	class ShaderObject final :public boost::noncopyable {
	public:
		ShaderObject(LPCWSTR filename, DX12::ShaderType shader_type);
		const void* GetBufferPointer();
		SIZE_T GetBufferSize();
	private:
		ComPtr<ID3DBlob> blob_;
	};
}