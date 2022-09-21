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
	class ShaderObject final {
	public:
		ShaderObject();
		//HACK:ƒƒ‚ƒŠ‚©‚ç‚Ì“Ç‚İ‚İ‚É‘Î‰‚·‚é
		ShaderObject(LPCWSTR filename, ShaderType type);
		bool IsValid() const;
		const void* GetBufferPointer() const;
		SIZE_T GetBufferSize() const;
	private:
		ComPtr<ID3DBlob> blob_;
	};
}
