//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �|���j
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "logo.h"
#include "number.h"
#include "remain.h"
#include "enemy.h"
#include "image.h"
#include "timer.h"
#include "score.h"

#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "player.h"
#include "meshfield.h"
#include "object.h"
#include "enemy.h"
#include "sky.h"
#include "particle.h"
#include "grass.h"
#include "sound.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CRemain * CResult::m_pRemain = NULL;
CTimer * CResult::m_pTimer = NULL;
CScore * CResult::m_pScore = NULL;
CScore * CResult::m_pScoreTime = NULL;
CScore * CResult::m_pScoreTotal = NULL;
CMeshField * CResult::m_pMeshField = NULL;
CCamera * CResult::m_pCamera = NULL;
CLight * CResult::m_pLight = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	m_nCounter = 0;
	m_nTime = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}
//=============================================================================
// ����������
//=============================================================================
void CResult::Init(void)
{
	m_nCounter = 0;
	m_nTime = 0;

	// �J�����N���X�̐���
	if (m_pCamera == NULL)
	{// ��
		m_pCamera = new CCamera;

		m_pCamera->Init();
	}

	// ���C�g�N���X�̐���
	if (m_pLight == NULL)
	{// ��
		m_pLight = new CLight;

		m_pLight->Init();
	}

	//======================
	// ���f���̓ǂݍ���
	//======================

	// �I�u�W�F�N�g�̓ǂݍ���
	CObject::LoadX();

	//======================
	// �e�N�X�`���̓ǂݍ���
	//======================

	// ���U���g���S�̓ǂݍ���
	CResultLogo::Load();

	// �i���o�[�e�N�X�`���̓ǂݍ���
	CNumber::Load();

	// UI�摜�̓ǂݍ���
	CImage::Load();


	//======================
	// ����
	//======================

	if (m_pMeshField == NULL)
	{// ���b�V���t�B�[���h�̐���	
		m_pMeshField = CMeshField::Create(D3DXVECTOR3(-500.0f, 0.0f, 500.0f));
	}

	//==============
	// X�̍���
	//==============
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -620.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -820.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -1020.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -1220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -1420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	//==============
	// X�̉E��
	//==============
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -620.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -820.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -1020.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -1220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -1420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	//==============
	// Z�̚�
	//==============
	CObject::Create(D3DXVECTOR3(150.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(350.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(550.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(750.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(950.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1150.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1350.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	//==============
	// Z�̎�O
	//==============
	CObject::Create(D3DXVECTOR3(150.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(350.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(550.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(750.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(950.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1150.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1350.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	// ��̐���
	CSky::Create(D3DXVECTOR3(750.0f, 480.0f, -750.0f));

	// ���U���g���S�N���X�̐���
	CResultLogo::Create(D3DXVECTOR3(150, 50, 0.0f),CResultLogo::RESULTTYPE_CLEAR);
}
//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//======================
	// ���f���̔j��
	//======================

	// �I�u�W�F�N�g�̔j��
	CObject::UnloadX();

	//======================
	// �e�N�X�`���̔j��
	//======================

	// ���U���g���S�̔j��
	CResultLogo::Unload();

	// �i���o�[�̃e�N�X�`���j��
	CNumber::Unload();

	// UI�摜�̃e�N�X�`���j��
	CImage::Unload();

	// �S�ĉ��
	CScene::ReleseAll();

	if (m_pRemain != NULL)
	{// �c�@��NULL����Ȃ�������NULL�ɂ���
		m_pRemain = NULL;
	}
	if (m_pTimer != NULL)
	{// �^�C�}�[��NULL����Ȃ�������NULL�ɂ���
		m_pTimer = NULL;
	}
	if (m_pScore != NULL)
	{// �X�R�A(�G)��NULL����Ȃ�������NULL�ɂ���
		m_pScore = NULL;
	}
	if (m_pScoreTime != NULL)
	{// �X�R�A(�^�C��)��NULL����Ȃ�������NULL�ɂ���
		m_pScoreTime = NULL;
	}
	if (m_pScoreTotal != NULL)
	{// ���v�X�R�A��NULL����Ȃ�������NULL�ɂ���
		m_pScoreTotal = NULL;
	}
	
	if (m_pMeshField != NULL)
	{// ���b�V���t�B�[���h��NULL����Ȃ�������NULL�ɂ���
		m_pMeshField = NULL;
	}

	//�@�J�����̔j��
	if (m_pCamera != NULL)
	{
		// �I������
		m_pCamera->Uninit();

		// �f�[�^�̊J��
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//�@���C�g�̔j��
	if (m_pLight != NULL)
	{
		// �I������
		m_pLight->Uninit();

		// �f�[�^�̊J��
		delete m_pLight;
		m_pLight = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	m_nCounter++;
	m_nTime++;

	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	// �Q�b�g�����G�̐��擾
	int nGetEnemy = CEnemy::GetCatchEnemy();

	// �~�߂����Ԃ̎擾
	int nTime = CTimer::GetStopTimer();

	if (m_nCounter == 60)
	{// 1�b��
		CImage::Create(D3DXVECTOR3(300, 200, 0), CImage::IMAGE_CATCHNUM);	// �߂܂�����

		if (m_pRemain == NULL)
		{// �T���̕߂܂������̐���

			m_pRemain = CRemain::Create(D3DXVECTOR3(700, 200, 0));
			m_pRemain->AddRemain(nGetEnemy);
			m_pRemain->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (m_pScore == NULL)
		{// �G�X�R�A�̐���
			m_pScore = CScore::Create(D3DXVECTOR3(1200, 200, 0));
			m_pScore->AddScore(nGetEnemy * 1000);
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JYAN);
	}
	if (m_nCounter == 120)
	{// 2�b��
		CImage::Create(D3DXVECTOR3(335, 400, 0), CImage::IMAGE_REMAINTIME);	// �c�莞��

		if (m_pTimer == NULL)
		{// �c�莞�Ԃ̐���
			m_pTimer = CTimer::Create(D3DXVECTOR3(700, 400, 0));
			m_pTimer->AddTime(nTime);
		}
		if (m_pScoreTime == NULL)
		{// �^�C���X�R�A�̐���
			m_pScoreTime = CScore::Create(D3DXVECTOR3(1200, 400, 0));
			m_pScoreTime->AddScore(nTime * 10);
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JYAN);
	}
	if (m_nCounter == 180)
	{// 3�b��
		CImage::Create(D3DXVECTOR3(410, 600, 0), CImage::IMAGE_TOTAL);		// ���v

		if (m_pScoreTotal == NULL)
		{// ���v�X�R�A
			m_pScoreTotal = CScore::CreateTotal(D3DXVECTOR3(1200, 600, 0));
			m_pScoreTotal->AddScore(nGetEnemy * 1000 + nTime * 10);
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JYAN);
	}
		 
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// �t�F�[�h�̎擾
	CFade *pFade = CManager::GetFade();

	if (m_nTime % 300 == 0)
	{// ��莞�Ԃ�������
		pFade->Set(CManager::MODE_RANKING, pFade->FADE_OUT);
		CManager::GetRankingScore(m_pScoreTotal->GetScore());
	}

	if (m_pCamera != NULL)
	{// �J�����̍X�V����
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ���C�g�̍X�V����
		m_pLight->Update();
	}

	///*�f�o�b�O�p*/
	//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{
	//	CManager::SetMode(CManager::MODE_RANKING);
	//}

	// �f�o�b�O�\��
	CDebugProc::Print(1, "���U���g\n");
}
//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
	if (m_pCamera != NULL)
	{// �J������ݒu
		m_pCamera->SetCamera();
	}
}