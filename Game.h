#pragma once

class Scene;
class Window;

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();
	Game();
	void ChangeScene(boost::shared_ptr<Scene> _scene);
	/// <summary>
	/// ���̃C���X�^���X��_windownum�Ԗڂ̃E�B���h�E��Ԃ�
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int _windownum);
	/// <summary>
	/// �E�B���h�E��Game�ɒǉ�����
	/// </summary>
	/// <returns>Game�ŉ��Ԗڂ̃E�B���h�E��(�E�B���h�E�̎w��ɂ͂��̔ԍ����g��)</returns>
	int AddWindow(WNDPROC _wndproc, LPCWSTR _classID, int _width, int _height,
		LPCWSTR _windowTitle);
	//1�t���[��������̎���
	const double mTimeEps = (double)1000 / (double)60;
	//�O�̏����Ƃ̎��ԍ������̎��Ԃ𒴂����ꍇ���̒l�ɋ�������
	const double mProcessTimeLimit = mTimeEps * 3;
private:
	void ProcessInput();
	void UpdateGame();
	bool GenerateOutput();
	bool mIsRunning;
	boost::shared_ptr<Scene> mCurrentScene;
	boost::shared_ptr<Scene> mPandingScene;
	bool mIsSceneChangable;
	std::vector<boost::shared_ptr<Window>> mWindows;
	void BeforeOutput();
	bool AfterOutput();
};