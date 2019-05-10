//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : �|���j
//
//=============================================================================
#include "timer.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "fade.h"
#include "score.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CTimer::m_nStopTimer = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTimer::CTimer()
{
	m_nTime = 0;
	m_nTimerCnt = 0;
	m_nFlashCnt = 0;
	m_bTimerStop = false;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos)
{
	m_nTime = 0;
	m_nTimerCnt = 0;
	m_nFlashCnt = 0;
	m_bTimerStop = false;

	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (TIME_INTERVAL_SIZE * nCount), pos.y, pos.z), TIMER_SIZE, TIMER_SIZE);
	}


	if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[��
		AddTime(TIMER_GAME);	// �^�C�}�[�ݒ�
	}
	else
	{// ����ȊO
		AddTime(TIMER_ZERO);		// �^�C�}�[�ݒ�
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount]->Uninit();

		// �������̊J��
		delete m_apNumber[nCount];

		// NULL������
		m_apNumber[nCount] = {};
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CTimer::Update(void)
{
	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		float AlphaCol = 1.0f;

		m_nTimerCnt++;

		if (m_bTimerStop == false)
		{// �^�C�}�[�������Ă�����
			if (m_nTime <= TIMER_RED)
			{// 30�b�ȉ��Ő�	
				if (m_nTime <= TIMER_TEN)
				{// 10�b�ȉ��œ_��
					m_nFlashCnt++;
					if (m_nFlashCnt <= TIMER_FLASH)
					{
						AlphaCol = 0.0f;
					}
					if (m_nFlashCnt >= TIMER_FLASH2)
					{
						m_nFlashCnt = 0;
					}
				}
				SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, AlphaCol));
			}

			if (m_nTimerCnt >= TIMER_SECOND)
			{	//�J�E���g������
				m_nTimerCnt = 0;

				m_nTime -= TIMER_ADD;
				//�X�V���邾��
				AddTime(TIMER_ZERO);
			}
		}
		if (m_bTimerStop == true)
		{// �^�C�}�[���~�܂�����
			m_nStopTimer = m_nTime;
		}
		if (m_nTime == TIMER_ZERO)
		{// �^�C�}�[��0�ɂȂ�����
			// �t�F�[�h�̎擾
			CFade *pFade = CManager::GetFade();
			pFade->Set(CManager::MODE_RESULT, pFade->FADE_OUT);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
//=============================================================================
// ���Z
//=============================================================================
void CTimer::AddTime(int nValue)
{
	m_nTime += nValue;

	int nAnswer = 1;
	int nTime;

	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		nTime = m_nTime % (nAnswer * TIMER_DEGIT) / nAnswer;
		m_apNumber[nCount]->SetNumber(nTime);
		nAnswer *= TIMER_DEGIT;
	}
}
//=============================================================================
// �F�ݒ�
//=============================================================================
void CTimer::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount]->SetColor(col);
	}
}
//=============================================================================
// �^�C�}�[�X�g�b�v
//=============================================================================
void CTimer::StopTimer(void)
{
	m_bTimerStop = true;

	// �X�R�A�̎擾
	CScore * pScore = CGame::GetScore();

	// �c�������ԕ��X�R�A���Z
	pScore->AddScore(m_nTime * TIMER_SCORE);
}
//=============================================================================
// �~�߂����Ԃ̎擾
//=============================================================================
int CTimer::GetStopTimer(void)
{
	return m_nStopTimer;
}
//=============================================================================
// ����
//=============================================================================
CTimer * CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer *pTimer = NULL;

	if (pTimer == NULL)
	{
		pTimer = new CTimer;

		if (pTimer != NULL)
		{
			pTimer->Init(pos);
		}
	}
	return pTimer;
}
