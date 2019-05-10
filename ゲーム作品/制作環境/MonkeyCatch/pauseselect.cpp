//=============================================================================
//
// �|�[�Y���ڏ��� [pauseselect.cpp]
// Author : �|���j
//
//=============================================================================
#include "pauseselect.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPauseSelect::m_apTexture[RESTART_MAX] = {};
CPauseSelect::RESTART CPauseSelect::m_Restart = RESTART_CONTINUE;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPauseSelect::CPauseSelect() : CScene(7)
{
	m_nSelect = 0;
	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		m_apLogo[nCntPauseSelect] = {};
		m_aCol[nCntPauseSelect] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPauseSelect::~CPauseSelect()
{

}
//=============================================================================
// �|�[�Y���ڏ���������
//=============================================================================
HRESULT CPauseSelect::Init(D3DXVECTOR3 pos)
{
	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		m_apLogo[nCntPauseSelect] = new CLogo(7);

		m_apLogo[nCntPauseSelect]->Init(D3DXVECTOR3(pos.x, pos.y - PAUSESELECT_INTERVAL + (nCntPauseSelect * PAUSESELECT_INTERVAL), 0.0f));
		m_apLogo[nCntPauseSelect]->BindTexture(m_apTexture[nCntPauseSelect]);

		m_aSelect[nCntPauseSelect] = PAUSESELECT_NONE;
		m_aCol[nCntPauseSelect] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_aSelect[0] = PAUSESELECT_SELECT;

	return S_OK;
}
//=============================================================================
// �|�[�Y���ڏI������
//=============================================================================
void CPauseSelect::Uninit(void)
{

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		m_apLogo[nCntPauseSelect]->Uninit();
		m_apLogo[nCntPauseSelect] = NULL;
	}
	// �I�u�W�F�N�g(�������g)�̔j��
	Release();

}
//=============================================================================
// �|�[�Y���ڍX�V����
//=============================================================================
void CPauseSelect::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	// �t�F�[�h�̎擾
	CFade *pFade = CManager::GetFade();

	// �I�����ڈړ�����
	if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_23) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_31) == true)
	{// S or ���@or ���X�e�B�b�N�� or �\���L�[���������ꂽ��
		m_aSelect[m_nSelect] = PAUSESELECT_NONE;
		m_nSelect = (m_nSelect + 1) % MAX_PAUSE_SELECT;
		m_aSelect[m_nSelect] = PAUSESELECT_SELECT;
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputKeyboard->GetTrigger(DIK_UP) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_21) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_29) == true)
	{// W or �� or ���X�e�B�b�N�� or �\���L�[���������ꂽ��
		m_aSelect[m_nSelect] = PAUSESELECT_NONE;
		m_nSelect = (m_nSelect + (MAX_PAUSE_SELECT - 1)) % MAX_PAUSE_SELECT;
		m_aSelect[m_nSelect] = PAUSESELECT_SELECT;
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);
	}

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		if (m_aSelect[nCntPauseSelect] == PAUSESELECT_SELECT)
		{// �I������Ă���Ƃ�
			m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{// �I������Ă��Ȃ��Ƃ�
			m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (m_apLogo[nCntPauseSelect] != NULL)
		{
			m_apLogo[nCntPauseSelect]->SetColor(m_aCol[m_nSelect]);
		}
	}
	// ��ʑJ��
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_1) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_2) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
	{// �Ώۂ̃{�^���������ꂽ��
		switch (m_nSelect)
		{
		case RESTART_CONTINUE:
			m_Restart = RESTART_CONTINUE; // �Q�[���ĊJ��Ԃ�
			break;
		case RESTART_RETRY:
			m_Restart = RESTART_RETRY;    // �Q�[����蒼����Ԃ�
			break;
		case RESTART_QUIT:
			m_Restart = RESTART_QUIT;     // �^�C�g���J�ڏ�Ԃ�
			break;
		}
	}
}
//=============================================================================
// �|�[�Y���ڕ`�揈��
//=============================================================================
void CPauseSelect::Draw(void)
{

}
//=============================================================================
// ���ڂ̎擾
//=============================================================================
CPauseSelect::RESTART CPauseSelect::GetRstart(void)
{
	return m_Restart;
}
//=============================================================================
// ����
//=============================================================================
CPauseSelect * CPauseSelect::Create()
{
	CPauseSelect *pPauseSelect = NULL;

	if (pPauseSelect == NULL)
	{
		pPauseSelect = new CPauseSelect;

		pPauseSelect->Init(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	}
	return pPauseSelect;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CPauseSelect::Load(void)
{
	CManager manager;	// �C���X�^���X

						// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PAUSESELECT_TEXTURE000, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PAUSESELECT_TEXTURE001, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, PAUSESELECT_TEXTURE002, &m_apTexture[2]);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPauseSelect::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MAX_PAUSE_SELECT; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
