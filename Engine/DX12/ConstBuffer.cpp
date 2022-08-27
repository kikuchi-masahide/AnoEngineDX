//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ConstBuffer.h"

DX12::ConstBuffer::ConstBuffer(ComPtr<ID3D12Device> device, D3D12_HEAP_TYPE heap_type, SIZE_T size, D3D12_RESOURCE_STATES state)
	:Buffer(device, heap_type, (size + 0xff) & ~0xff, state)
{
}
