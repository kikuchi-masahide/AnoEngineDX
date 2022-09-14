//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	enum class ShaderType :int{
		VertexShader,
		PixelShader
	};
	class ShaderObject final :public boost::noncopyable {
	public:
		ShaderObject(LPCWSTR filename, ShaderType type);
		const void* GetBufferPointer();
		SIZE_T GetBufferSize();
	private:
		ComPtr<ID3DBlob> blob_;
	};
}
