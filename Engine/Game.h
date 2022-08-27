//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "DX12/Master.h"
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
		//HACK:�����̒x���������A����уR���X�g���N�^��panding�ɓ��ꂽ�̂�Update�O�ɖ{�z��Ɉڂ�����
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
	void AddWindow(WNDPROC wndproc, LPCWSTR classID, int width, int height, LPCWSTR windowTitle, int windowid, bool use_swapchain = true);
	/// <summary>
	/// windowid�ɑΉ�����E�B���h�E�̎Q�Ƃ�Ԃ�
	/// </summary>
	boost::shared_ptr<Window> GetWindow(int windowid) const;
	/// <summary>
	/// ����windowid��render target�ɂ���R�}���h���Acmd_list�ɃZ�b�g����B
	/// </summary>
	void SetRenderTarget(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid);
	/// <summary>
	/// ����windowid��backbuffer��state���Abefore����after�֕ύX����
	/// </summary>
	void SetBackbufferStateBarrier(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid,
		D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
	/// <summary>
	/// ����windowid��backbuffer���A�w��F�ŃN���A����
	/// </summary>
	void ClearBackbuffer(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid,
		float r, float g, float b);
	/// <summary>
	/// �w��windowid��depth stencil buffer��value�ŃN���A����
	/// </summary>
	void ClearDepthStencilBuffer(std::shared_ptr<DX12::GraphicsCommandList> cmd_list, int windowid,
		float value);
	std::shared_ptr<DX12::SwapChain> GetSwapChain(int windowid) const;
	std::shared_ptr<DX12::CommandQueue> GetCommandQueue() const;
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
	//���̃|�C���^��delete���f�X�g���N�^���Ă�
	void DeleteScene(Scene* scene);
	Scene* current_scene_;
	Scene* panding_scene_;
	bool is_scene_changable_;
	std::map<unsigned int, boost::shared_ptr<Window>> windows_;
	std::map<unsigned int, std::shared_ptr<DX12::SwapChain>> swapchains_;
	std::map<unsigned int, std::shared_ptr<DX12::DepthStencilBuffer>> dsbuffers_;
	std::map<unsigned int, std::shared_ptr<DX12::DescriptorHeap>> dsv_descheaps_;
	//swapchains_�̍쐬�ȂǂɎg��commandqueue
	std::shared_ptr<DX12::CommandQueue> graphics_cmd_queue_;
	int current_swapchain_id_;
	InputSystem input_system_;
	bool is_executing_destructor_;
	bool terminate_flag_;
};