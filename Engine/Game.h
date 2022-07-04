//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12/DX12.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "InputSystem.h"

class Scene;
class Window;

//HACK:�����������Ȃǂ�Game�Ŋ������������������₷���ȂǁCGame��h�������N���X�Ɏ������悹��`�ɂ��������x�^�[��?
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
		if (is_executing_destructor_) {
			return;
		}
		if (!is_scene_changable_) {
			if (panding_scene_ != nullptr)DeleteScene(panding_scene_);
			panding_scene_ = DBG_NEW S(this, args...);
		}
		else {
			if (current_scene_ != nullptr)DeleteScene(current_scene_);
			current_scene_ = DBG_NEW S(this, args...);
		}
	};
	/// <summary>
	/// �E�B���h�E��Game�ɒǉ�����
	/// </summary>
	/// <param name="_windowid">�񕉐����l�E�B���h�EID(�E�B���h�E�C�X���b�v�`�F�[���̎w��ɂ͂���ID���g��)</param>
	void AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height, LPCWSTR windowTitle, int windowid);
	/// <summary>
	/// windowid�ɑΉ�����E�B���h�E�̎Q�Ƃ�Ԃ�
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int windowid) const;
	/// <summary>
	/// �Q�[�����[�v���J�n���� main.cpp�ŌĂяo��
	/// </summary>
	void RunLoop();
	/// <summary>
	/// ���݊J���Ă���X���b�v�`�F�[������C���̃E�B���h�E�ɕt������X���b�v�`�F�[�����J��
	/// </summary>
	void OpenSwapChain(int windowid);
	/// <summary>
	/// ���݊J���Ă���X���b�v�`�F�[�������
	/// </summary>
	void CloseSwapChain();
	/// <summary>
	/// ����Output�I����RunLoop��E�o���C�v���O�������I��������
	/// </summary>
	void Terminate();
	DX12 dx12_;
	//HACK:DX12�Ɋ܂߂邩�ۂ��l����
	TextureManager tex_manager_;
	ShaderManager shader_manager_;
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
	//���̃|�C���^��delete���f�X�g���N�^���Ă�
	void DeleteScene(Scene* scene);
	Scene* current_scene_;
	Scene* panding_scene_;
	bool is_scene_changable_;
	std::map<unsigned int, boost::shared_ptr<Window>> windows_;
	std::map<unsigned int, boost::shared_ptr<DX12SwapChain>> swapchains_;
	int current_swapchain_id_;
	InputSystem input_system_;
	bool is_executing_destructor_;
	bool terminate_flag_;
};