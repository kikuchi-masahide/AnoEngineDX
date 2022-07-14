//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	enum class ResourceBarrierState:char {
		PRESENT,
		RENDER_TARGET,
		COPY_DEST,
		PIXEL_SHADER_RESOURCE,
		size
	};
}
