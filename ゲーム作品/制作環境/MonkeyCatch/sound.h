//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CSound
{
public:
	//*****************************************************************************
	// �p�����[�^�\���̒�`
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum //���x���Ɛ��ƕ��т𑵂���
	{
		SOUND_LABEL_BGM_GAME = 0,
		SOUND_LABEL_BGM_TITLE,
		SOUND_LABEL_BGM_TUTORIAL,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_SE_MUSIAMI,
		SOUND_LABEL_SE_JUMP,
		SOUND_LABEL_SE_JYAN,
		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_ENETR,
		SOUND_LABEL_SE_OPEN,
		SOUND_LABEL_SE_CLOSE,
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	virtual HRESULT Init(HWND hWnd);
	virtual void Uninit(void);

	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

																// �e���f�ނ̃p�����[�^ ���x���Ɛ��ƕ��т𑵂���
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/BGM_game.wav",-1 },		// �Q�[��BGM
		{ "data/BGM/BGM_title.wav",-1 },	// �^�C�g��BGM / �����L���O
		{ "data/BGM/BGM_tutorial.wav",-1 },	// �`���[�g���A��BGM
		{ "data/BGM/BGM_result.wav",-1 },	// ���U���gBGM
		{ "data/SE/musiami.wav", 0 },		// ����
		{ "data/SE/jump.wav", 0 },			// �W�����v
		{ "data/SE/jyan.wav", 0 },			// �W���[��
		{ "data/SE/select.wav", 0 },		// �I����
		{ "data/SE/enter.wav", 0 },			// ���艹
		{ "data/SE/open.wav", 0 },			// �|�[�Y�J����
		{ "data/SE/close.wav", 0 },			// �|�[�Y���鉹
	};
};
#endif
