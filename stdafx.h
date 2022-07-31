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

//正直ここに置きたくないが，シェーダとの情報の受け渡しのために仕方なく
#include <DirectXMath.h>
//一部構造体のみ名前空間を外し使う構造体を分かりやすくする
using DirectX::XMMATRIX;
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;

//DBG_NEWを用いるカスタムアロケータ
//HACK:今はメモリプールなどは特に考えず常にDBG_NEWを使うのでとりあえずこれにしているが、
//メモリプールなどを使いそうになったら訂正する
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
