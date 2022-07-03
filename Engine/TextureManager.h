#pragma once

class Game;
class DX12Resource;
class DX12DescriptorHeap;

class TextureManager :public boost::noncopyable {
public:
	//コンストラクタ呼び出しをGameに制限
	TextureManager(Game* const _game);
	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="_desc">ディスクリプタヒープ</param>
	/// <param name="_num">ディスクリプタヒープ上のどこにディスクリプタを置くか</param>
	/// <param name="_textureid">テクスチャID</param>
	void LoadTexture(const wchar_t* _filename, unsigned int _textureid);
	/// <summary>
	/// テクスチャのアンロード(DX12Resourceの解放)
	/// </summary>
	void UnloadTexture(unsigned int _textureid);
	/// <summary>
	/// DX12Resourceを取得
	/// </summary>
	boost::shared_ptr<DX12Resource> GetDX12Resource(unsigned int _textureid);
	/// <summary>
	/// ディスクリプタヒープに，テクスチャのSRVを作る
	/// </summary>
	/// <param name="heap_ind">DescriptorHeapの何番目にSRVを作るか</param>
	void CreateSRVof(unsigned int texture_id, boost::shared_ptr<DX12DescriptorHeap> desc_heap, unsigned int heap_ind);
private:
	std::map<unsigned int, boost::shared_ptr<DX12Resource>> mTexturesmap;
	std::map<unsigned int, unsigned char> mFormat;
	Game* const mGame;
};