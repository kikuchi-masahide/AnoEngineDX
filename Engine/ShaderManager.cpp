#include "ShaderManager.h"
#include "Game.h"
#include "Log.h"

ShaderManager::ShaderManager(Game* const _game)
	:mGame(_game)
{
}

void ShaderManager::LoadShader(const wchar_t* _filename, DX12Config::ShaderType _type, unsigned int _shaderid)
{
	//ShaderID重複チェック
	BOOST_ASSERT_MSG(mShadersmap.find(_shaderid) == mShadersmap.end(), "ShaderID duplicating");
	auto shader = mGame->dx12_.LoadShader(_filename, _type);
	mShadersmap[_shaderid] = shader;
}

void ShaderManager::UnloadShader(unsigned int _shaderid)
{
	auto itr = mShadersmap.find(_shaderid);
	if (itr != mShadersmap.end())
	{
		mShadersmap.erase(itr);
	}
}

boost::shared_ptr<DX12ShaderObject> ShaderManager::GetDX12ShaderObject(unsigned int _shaderid)
{
	auto itr = mShadersmap.find(_shaderid);
	if (itr != mShadersmap.end())
	{
		return itr->second;
	}
	else
	{
		std::string str("unregistered ShaderID:");
		str += std::to_string(_shaderid);
		Log::OutputCritical(str);
		return nullptr;
	}
}
