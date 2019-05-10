//=============================================================================
//
// �c�@���� [remain.h]
// Author : �|���j
//
//=============================================================================
#include "remain.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRemain::CRemain()
{
	m_nRemain = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRemain::~CRemain()
{
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CRemain::Init(D3DXVECTOR3 pos)
{
	m_nRemain = 0;

	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[��
		for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
		{
			m_apNumber[nCount] = new CNumber;

			m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (REMAIN_INTERVAL_SIZE * nCount), pos.y, pos.z), REMAIN_SIZE, REMAIN_SIZE);
		}

		// �G�̑����擾
		m_NumEnemy = CEnemy::GetNumEnemy();
		AddRemain(m_NumEnemy);
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (mode == CManager::MODE_RESULT)
	{// ���[�h�����U���g
		for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
		{
			m_apNumber[nCount] = new CNumber;

			m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (R_REMAIN_INTERVAL_SIZE * nCount), pos.y, pos.z), R_REMAIN_SIZE, R_REMAIN_SIZE);
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CRemain::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
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
void CRemain::Update(void)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		m_apNumber[nCount]->Update();
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CRemain::Draw(void)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
//=============================================================================
// �c�@�̉��Z
//=============================================================================
void CRemain::AddRemain(int nValue)
{
	m_nRemain += nValue;

	int nAnswer = 1;
	int nRemain;

	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		nRemain = m_nRemain % (nAnswer * REMAIN_DEGIT) / nAnswer;
		m_apNumber[nCount]->SetNumber(nRemain);
		nAnswer *= REMAIN_DEGIT;
	}
}
//=============================================================================
// �F�ݒ�
//=============================================================================
void CRemain::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		m_apNumber[nCount]->SetColor(col);
	}
}

//=============================================================================
// �c�@�̐���
//=============================================================================
CRemain * CRemain::Create(D3DXVECTOR3 pos)
{
	CRemain *pRemain = NULL;

	if (pRemain == NULL)
	{
		pRemain = new CRemain;

		if (pRemain != NULL)
		{
			pRemain->Init(pos);
		}
	}
	return pRemain;
}
