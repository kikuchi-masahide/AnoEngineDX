//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12/Master.h"
#include "InputSystem.h"

class Scene;
class Window;

class Game {
public:
	//1�t���[��������̎���
	static constexpr double kFrameTimeDelta = 1000.0 / 60;
	//�O�̏����Ƃ̎��ԍ������̎��Ԃ𒴂����ꍇ�A���̒l�ɋ�������
	static constexpr double kProcessTimeDeltaLimit = kFrameTimeDelta * 3;
	Game();
	virtual ~Game();
	/// <summary>
	/// ���t���[������ǂ̃V�[���Ɉڍs���邩�w�肷��D
	/// (���ۂɃV�[�����ς��͎̂��t���[�����炾���C���V�[���̏��������s����̂͌Ăяo�����Ȃ̂�����)
	/// </summary>
	template<class S, class... Args>
	void ChangeScene(Args... args) {
		//HACK:�����̒x���������A����уR���X�g���N�^��panding�ɓ��ꂽ�̂�Update�O�ɖ{�z��Ɉڂ�����
		if (is_executing_destructor_) {
			return;
		}
		//���ݎ��s���̏ꍇ���f��ʍ�
		async_initing_thread_.interrupt();
		async_initing_thread_.join();
		Scene* newscene = DBG_NEW S(this, args...);
		boost::thread th(&Game::AsyncInitializeScene, this, newscene);
		async_initing_thread_.swap(th);
	};
	/// <summary>
	/// �E�B���h�E��Game�ɒǉ�����
	/// </summary>
	/// <param name="_windowid">�����l�E�B���h�EID(�E�B���h�E�C�X���b�v�`�F�[���̎w��ɂ͂���ID���g��)</param>
	/// <returns>�쐬����window</returns>
	std::weak_ptr<Window> AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height,
		LPCWSTR windowTitle, int windowid);
	/// <summary>
	/// windowid�ɑΉ�����E�B���h�E�̃|�C���^��Ԃ�
	/// </summary>
	std::shared_ptr<Window> GetWindow(int windowid) const;
	/// <summary>
	/// �Q�[�����[�v���J�n���� main.cpp�ŌĂяo��
	/// </summary>
	void RunLoop();
	/// <summary>
	/// ����Output�I����RunLoop��E�o���C�v���O�������I��������
	/// </summary>
	void Terminate();
	DX12::Master dx12_;
protected:
	/// <summary>
	/// UpdateGame���ŁAScene��1���Update���s���O�̏������s��
	/// </summary>
	virtual void BeforeUpdate();
	/// <summary>
	/// UpdateGame���ŁAScene��1���Update���s������̏������s��
	/// </summary>
	virtual void AfterUpdate();
	/// <summary>
	/// GenerateOutput���ŁAScene��Output���s���O�̏������s��
	/// </summary>
	virtual void BeforeOutput();
	/// <summary>
	/// GenerateOutput���ŁAScene��Output���s������̏������s��
	/// </summary>
	virtual void AfterOutput();
private:
	/// <summary>
	/// ���͏���(2�t���[�����ȏ�̍X�V�������s���ꍇ�A�L�[���͂�1�t���[���ڂ̍X�V���O�̂ݍs�����߁ABeforeUpdate�ɂ͊܂߂Ȃ�)
	/// </summary>
	void ProcessInput();
	/// <summary>
	/// �Q�[���S�̂̍X�V����
	/// </summary>
	void UpdateGame();
	/// <summary>
	/// �Q�[���S�̂̏o�͐���
	/// </summary>
	void GenerateOutput();
	/// <summary>
	/// ChangeScene�ɂĕʃX���b�h�Ŏ��s����Ascene��AsyncInitialize�����s����֐�
	/// </summary>
	/// <param name="scene"></param>
	void AsyncInitializeScene(Scene* scene);
	//�t���[���̏��߂ŁAscene_���X�V
	void ProcessPandingScene();
	//���̃|�C���^��delete���f�X�g���N�^���Ă�
	void DeleteScene(Scene* scene);
	Scene* current_scene_;
	Scene* panding_scene_;
	boost::mutex panding_scene_mutex_;
	std::map<unsigned int, std::shared_ptr<Window>> windows_;
	int current_swapchain_id_;
	InputSystem input_system_;
	bool is_executing_destructor_;
	bool terminate_flag_;
	//AsyncInitializeScene�𓮂����Ă���X���b�h
	boost::thread async_initing_thread_;
};