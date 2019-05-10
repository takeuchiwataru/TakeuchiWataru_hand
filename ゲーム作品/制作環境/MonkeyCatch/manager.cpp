//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : �|���j
//
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "fade.h"

#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "tutorial.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CRenderer * CManager::m_pRenderer = NULL;
CDebugProc * CManager::m_pDebugProc = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CInputJoyPad * CManager::m_pInputJoyPad = NULL;
CSound * CManager::m_pSound = NULL;
//CCamera * CManager::m_pCamera = NULL;
//CLight * CManager::m_pLight = NULL;

CManager::MODE CManager::m_mode = MODE_TITLE;
CTitle * CManager::m_pTitle = NULL;
CGame * CManager::m_pGame = NULL;
CResult * CManager::m_pResult = NULL;
CRanking * CManager::m_pRanking = NULL;
CTutorial * CManager::m_pTutorial = NULL;
CFade * CManager::m_pFade = NULL;
int CManager::m_aScore[MAX_RANKING] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	MODE m_mode = MODE_TITLE;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_�����O�N���X�̐���
	if (m_pRenderer == NULL)
	{// ��
		m_pRenderer = new CRenderer;

		m_pRenderer->Init(hWnd,true);
	}

	// �f�o�b�O�\���N���X�̐���
	if (m_pDebugProc == NULL)
	{// ��
		m_pDebugProc = new CDebugProc;

		m_pDebugProc->Init();
	}

	// �L�[�{�[�h�N���X�̐���
	if (m_pInputKeyboard == NULL)
	{// ��
		m_pInputKeyboard = new CInputKeyboard;

		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// �W���C�p�b�h�N���X�̐���
	if (m_pInputJoyPad == NULL)
	{// ��
		m_pInputJoyPad = new CInputJoyPad;

		m_pInputJoyPad->Init(hInstance, hWnd);
	}

	// ���̐���
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;

		m_pSound->Init(hWnd);
	}

	// �t�F�[�h�̐���
	m_pFade = CFade::Create();
	m_pFade->Set(m_mode, m_pFade->FADE_IN);

	//// ���[�h�̐ݒ�
	//SetMode(m_mode);

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	//�@�����_�����O�N���X�̔j��
	if (m_pRenderer != NULL)
	{
		// �I������
		m_pRenderer->Uninit();

		// �f�[�^�̊J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�@�f�o�b�O�\���N���X�j��
	if (m_pDebugProc != NULL)
	{
		// �I������
		m_pDebugProc->Uninit();

		// �f�[�^�̊J��
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	//�@�L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		// �I������
		m_pInputKeyboard->Uninit();

		// �f�[�^�̊J��
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//�@�W���C�p�b�h�̔j��
	if (m_pInputJoyPad != NULL)
	{
		// �I������
		m_pInputJoyPad->Uninit();

		// �f�[�^�̊J��
		delete m_pInputJoyPad;
		m_pInputJoyPad = NULL;
	}

	// �T�E���h�̔j��
	if (m_pSound != NULL)
	{
		// �I������
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	// �e���[�h�̏I������
	if (m_pTitle != NULL)
	{
		m_pTitle->Uninit();

		delete m_pTitle;
		m_pTitle = NULL;
	}

	if (m_pTutorial != NULL)
	{
		m_pTutorial->Uninit();

		delete m_pTutorial;
		m_pTutorial = NULL;
	}

	if (m_pGame != NULL)
	{
		m_pGame->Uninit();

		delete m_pGame;
		m_pGame = NULL;
	}

	if (m_pResult != NULL)
	{
		m_pResult->Uninit();

		delete m_pResult;
		m_pResult = NULL;
	}

	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();

		delete m_pRanking;
		m_pRanking = NULL;
	}

	// �t�F�[�h�̔j��
	if (m_pFade != NULL)
	{
		// �I������
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{// �����_�����O�N���X�̍X�V����
		m_pRenderer->Update();
	}
	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h�̍X�V����
		m_pInputKeyboard->Update();
	}
	if (m_pInputJoyPad != NULL)
	{// �W���C�p�b�h�̍X�V����
		m_pInputJoyPad->Update();
	}

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Update();
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Update();
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Update();
		}
		break;

	}
	if (m_pFade != NULL)
	{// �t�F�[�h�̍X�V����
		m_pFade->Update();
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// �����_�����O�N���X�̕`�揈��
		m_pRenderer->Draw();
	}

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Draw();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Draw();
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Draw();
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Draw();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Draw();
		}
		break;
	}
}
//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();

			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();

			delete m_pGame;
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();

			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	}

	m_mode = mode;	//���݂̃��[�h��؂�ւ���

	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{
			m_pTitle = new CTitle;

			m_pTitle->Init();
			pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{
			m_pTutorial = new CTutorial;

			m_pTutorial->Init();

			pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{
			m_pGame = new CGame;

			m_pGame->Init();
			pSound->StopSound(CSound::SOUND_LABEL_BGM_TUTORIAL);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{
			m_pResult = new CResult;

			m_pResult->Init();
			pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
		}
		break;

	case MODE_RANKING:
		if (m_pRanking == NULL)
		{
			m_pRanking = new CRanking;

			m_pRanking->Init();
			pSound->StopSound(CSound::SOUND_LABEL_BGM_RESULT);

		}
		break;
	}
}
//=============================================================================
// ���[�h�̎擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}
//=============================================================================
// �����_�����O�̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// �L�[�{�[�h�̎擾
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================================================
// �W���C�p�b�h�̎擾
//=============================================================================
CInputJoyPad *CManager::GetJoyPad(void)
{
	return m_pInputJoyPad;
}
//=============================================================================
// �T�E���h�̎擾
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// �t�F�[�h�̎擾
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}
//=============================================================================
// �����L���O�X�R�A�̎擾
//=============================================================================
int CManager::GetRanking(int index)
{
	return m_aScore[index];
}
//=============================================================================
// �����L���O�X�R�A�̎擾
//=============================================================================
void CManager::GetRankingScore(int nScore)
{
	int nData = 0;

	if (m_aScore[MAX_RANKING - 1] <= nScore)
	{
		m_aScore[MAX_RANKING - 1] = nScore;
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		for (int nNumber = 0; nNumber < MAX_RANKING - 1; nNumber++)
		{
			if (m_aScore[nNumber] < m_aScore[nNumber + 1])
			{// ����ւ�
				nData = m_aScore[nNumber];
				m_aScore[nNumber] = m_aScore[nNumber + 1];
				m_aScore[nNumber + 1] = nData;
			}
		}
	}
}


