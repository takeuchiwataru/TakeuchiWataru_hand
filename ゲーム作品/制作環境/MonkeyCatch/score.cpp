//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �|���j
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore()
{
	m_nScore = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (SCORE_INTERVAL_SIZE * nCount), pos.y, pos.z), SCORE_SIZE, SCORE_SIZE);
	}

	m_nScore = 0;

	return S_OK;
}
//=============================================================================
// ����������(���v�X�R�A�p)
//=============================================================================
HRESULT CScore::InitTotal(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (90 * nCount), pos.y, pos.z), 50, 50);
	}

	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
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
void CScore::Update(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Update();
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}
//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	int nAnswer = 1;
	int nScore;

	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		nScore = m_nScore % (nAnswer * SCORE_DEGIT) / nAnswer;
		m_apNumber[nCount]->SetNumber(nScore);
		nAnswer *= SCORE_DEGIT;
	}
}
//=============================================================================
// �F�ݒ�
//=============================================================================
void CScore::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->SetColor(col);
	}
}
//=============================================================================
// �X�R�A�̐���
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->Init(pos);
		}
	}
	return pScore;
}
//=============================================================================
// �X�R�A�̐���(���v)
//=============================================================================
CScore * CScore::CreateTotal(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->InitTotal(pos);
		}
	}
	return pScore;
}
