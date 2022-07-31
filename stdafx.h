//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <cmath>
#include <string>
#include <map>
#include <Windows.h>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <queue>
#include <typeinfo>
#include <random>
#include <optional>
#include <memory>
#include <source_location>

#include "boost/noncopyable.hpp"

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "boost/make_shared.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/pool/pool.hpp"
#include "boost/thread.hpp"

//���������ɒu�������Ȃ����C�V�F�[�_�Ƃ̏��̎󂯓n���̂��߂Ɏd���Ȃ�
#include <DirectXMath.h>
//�ꕔ�\���̂̂ݖ��O��Ԃ��O���g���\���̂𕪂���₷������
using DirectX::XMMATRIX;
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;

//DBG_NEW��p����J�X�^���A���P�[�^
//HACK:���̓������v�[���Ȃǂ͓��ɍl�������DBG_NEW���g���̂łƂ肠��������ɂ��Ă��邪�A
//�������v�[���Ȃǂ��g�������ɂȂ������������
template<class T>
class DBG_NEW_allocator {
public:
	using value_type = T;
	DBG_NEW_allocator() {}
	DBG_NEW_allocator(const DBG_NEW_allocator&) {}
	DBG_NEW_allocator(DBG_NEW_allocator&&){}
	template<class U> DBG_NEW_allocator(const DBG_NEW_allocator<U>&){}
	[[nodiscard]] T* allocate(std::size_t num)
	{
		T* const p = DBG_NEW T[num];
		return p;
	}
	void deallocate(T* p, std::size_t num) {
		delete[] p;
	}
};

template<class T1,class T2>
bool operator==(const DBG_NEW_allocator<T1>&, const DBG_NEW_allocator<T2>&)
{
	return true;
}

template<class T1,class T2>
bool operator!=(const DBG_NEW_allocator<T1>&, const DBG_NEW_allocator<T2>&)
{
	return false;
}
