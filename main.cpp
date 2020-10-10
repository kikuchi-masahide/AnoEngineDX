#include <Windows.h>

#ifdef _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif // _DEBUG
	Log::Init();
	Log::OutputTrivial(std::string("Log output"));
	Log::OutputCritical(std::string("Log critical"));
	return 0;
}