#include "window.h"
#include "Game.h"
#include "Scene1.h"

LRESULT WindowProcedure(HWND, UINT, WPARAM, LPARAM);
boost::shared_ptr<DX12Resource> InitiateVertexBuffer(Game* _game);

#ifdef _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif // _DEBUG
	Log::OutputTrivial("program entry");
	Game* game = new Game;
	if (!game->Initialize())return -1;
	game->AddWindow((WNDPROC)WindowProcedure, TEXT("WndClass"), 1024, 768, TEXT("Title here"));
	auto windowptr = game->GetWindow(0);
	boost::shared_ptr<DX12DescriptorHeap> swapchaindescheap
		= game->mdx12.CreateDescriptorHeap(DX12Config::DescriptorHeapType::RTV, DX12Config::ShaderVisibility::NONE, 2);
	game->mdx12.CreateSwapChain(windowptr->GetWindowHandle(), 1024, 768,swapchaindescheap);
	game->ChangeScene<Scene1>();
	//頂点バッファ(仮置き)
	InitiateVertexBuffer(game);
	auto vertex = game->mdx12.LoadShader(L"BasicVertexShader.hlsl", DX12Config::ShaderType::VERTEX);
	auto pixel = game->mdx12.LoadShader(L"BasicPixelShader.hlsl", DX12Config::ShaderType::PIXEL);
	game->RunLoop();
	game->Shutdown();

	delete game;
	return 0;
}

boost::shared_ptr<DX12Resource> InitiateVertexBuffer(Game* _game) {
	Vector3 vertices[] = {
		GetVector3(-1,-1,0),
		GetVector3(-1,1,0),
		GetVector3(1,-1,0)
	};
	auto resource = _game->mdx12.CreateVertexBuffer(sizeof(float) * 3 * 3);
	DX12VertexLayout layout;
	layout.push_back(DX12VertexLayoutUnit("POSITION", DX12Config::VertexLayoutFormat::R32G32B32_FLOAT, 0, DX12Config::VertexLayoutInputClassification::INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1));
	return resource;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}