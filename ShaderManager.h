#pragma once

class Game;
class DX12ShaderObject;
namespace DX12Config {
	enum class ShaderType :unsigned char;
}

/// <summary>
/// Gameの中でシェーダを管理するクラス
/// </summary>
class ShaderManager :public boost::noncopyable
{
public:
	ShaderManager(Game* const _game);
	void LoadShader(const wchar_t* _filename, DX12Config::ShaderType _type, unsigned int _shaderid);
	void UnloadShader(unsigned int _shaderid);
	boost::shared_ptr<DX12ShaderObject> GetDX12ShaderObject(unsigned int _shaderid);
private:
	std::map<unsigned int, boost::shared_ptr<DX12ShaderObject>> mShadersmap;
	Game* const mGame;
};