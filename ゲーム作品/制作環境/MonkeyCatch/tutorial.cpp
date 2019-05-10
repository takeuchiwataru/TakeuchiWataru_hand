//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : �|���j
//
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "fade.h"

#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "player.h"
#include "meshfield.h"
#include "meshorbit.h"
#include "object.h"
#include "enemy.h"
#include "sky.h"
#include "meshwall.h"
#include "particle.h"
#include "grass.h"

#include "number.h"
#include "score.h"
#include "frame.h"
#include "remain.h"
#include "image.h"
#include "timer.h"
#include "polygon.h"
#include "logo.h"
#include "image.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CPlayer *CTutorial::m_pPlayer = NULL;
CMeshField * CTutorial::m_pMeshField = NULL;
CScore * CTutorial::m_pScore = NULL;
CRemain * CTutorial::m_pRemain = NULL;
CCamera * CTutorial::m_pCamera = NULL;
CLight * CTutorial::m_pLight = NULL;
CTimer * CTutorial::m_pTimer = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}
//=============================================================================
// ����������
//=============================================================================
void CTutorial::Init(void)
{
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

	// �`���[�g���A�����S�̓ǂݍ���
	CTutorialLogo::Load();

	// �v���X�G���^�[�̓ǂݍ���
	CPressEnter::Load();

	// �摜�̓ǂݍ���
	CImage::Load();

	// �i���o�[�e�N�X�`���̓ǂݍ���
	CNumber::Load();

	// �t���[���̓ǂݍ���
	CFrame::Load();

	// �p�[�e�B�N���̓ǂݍ���
	CParticle::Load();

	//======================
	// ����
	//======================
	if (m_pPlayer == NULL)
	{// �v���C���[�̐���
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(750.0f, 0.0f, -750.0f));
	}

	if (m_pMeshField == NULL)
	{// ���b�V���t�B�[���h�̐���	
		m_pMeshField = CMeshField::Create(D3DXVECTOR3(-500.0f, 0.0f, 500.0f));
	}

	// �G�̐���
	CEnemy::Create(D3DXVECTOR3(550.0f, 0.0f, -750.0f), CEnemy::ENEMYTYPE_000);

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
	CSky::Create(D3DXVECTOR3(750.0f, 500.0f, -750.0f));

	//======================
	// UI
	//======================

	if (m_pScore == NULL)
	{// �X�R�A�N���X�̐���
		m_pScore = CScore::Create(D3DXVECTOR3(1200, 80, 0));
	}

	// �X�R�A�t���[���̐���
	CFrame::Create(D3DXVECTOR3(1000, 80, 0), CFrame::FRAMETYPE_SCORE);

	// �`���[�g���A�����S�̐���
	CTutorialLogo::Create(D3DXVECTOR3(150, 50, 0.0f));

	// �v���X�G���^�[�N���X�̐���
	CPressEnter::Create(D3DXVECTOR3(1120, 700, 0.0f), CPressEnter::PRESSENTER_TUTORIAL);
	CPressEnter::DispPressEnter();

	// �R���g���[���[�̐���
	CImage::Create(D3DXVECTOR3(200, 550, 0), CImage::IMAGE_TUTORIAL);

	// �^�C�}�[�t���[���̐���
	CFrame::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 10, 80, 0), CFrame::FRAMETYPE_TIMER);

	if (m_pTimer == NULL)
	{// �^�C�}�[�̐���
		m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 80, 0));
	}
}
//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	//======================
	// ���f���̔j��
	//======================

	// �I�u�W�F�N�g�̔j��
	CObject::UnloadX();

	//======================
	// �e�N�X�`���̔j��
	//======================

	// �i���o�[�̃e�N�X�`���j��
	CNumber::Unload();

	// �t���[���̔j��
	CFrame::Unload();

	// �p�[�e�B�N���̃e�N�X�`���j��
	CParticle::Unload();

	// �`���[�g���A�����S�e�N�X�`���j��
	CTutorialLogo::Unload();

	// �v���X�G���^�[�e�N�X�`���j��
	CPressEnter::Unload();

	// �摜�̔j��
	CImage::Unload();

	// �S�ĉ��
	CScene::ReleseAll();

	if (m_pPlayer != NULL)
	{// �v���C���[��NULL����Ȃ�������NULL�ɂ���
		m_pPlayer = NULL;
	}

	if (m_pMeshField != NULL)
	{// ���b�V���t�B�[���h��NULL����Ȃ�������NULL�ɂ���
		m_pMeshField = NULL;
	}

	if (m_pScore != NULL)
	{// �X�R�A��NULL����Ȃ�������NULL�ɂ���
		m_pScore = NULL;
	}
	if (m_pRemain != NULL)
	{// �c�@��NULL����Ȃ�������NULL�ɂ���
		m_pRemain = NULL;
	}
	if (m_pTimer != NULL)
	{// �^�C�}�[��NULL����Ȃ�������NULL�ɂ���
		m_pTimer = NULL;
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
void CTutorial::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// �t�F�[�h�̎擾
	CFade *pFade = CManager::GetFade();

	if (pInputJoyPad != NULL)
	{
		if (pInputKeyboard->GetTrigger(DIK_1) == true||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_12) == true)
		{// �Ώۂ̃{�^���������ꂽ��(1,START�{�^��)
			pFade->Set(CManager::MODE_GAME, pFade->FADE_OUT);
		}
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
	//	CManager::SetMode(CManager::MODE_GAME);
	//}

	// �f�o�b�O�\��
	CDebugProc::Print(1, "�`���[�g���A��\n");
}
//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	if (m_pCamera != NULL)
	{// �J������ݒu
		m_pCamera->SetCamera();
	}
}
//=============================================================================
// ���b�V���t�B�[���h�̎擾
//=============================================================================
CMeshField *CTutorial::GetMeshField(void)
{
	return m_pMeshField;
}
//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}
//=============================================================================
// �c�@�̎擾
//=============================================================================
CRemain *CTutorial::GetRemain(void)
{
	return m_pRemain;
}
//=============================================================================
// �X�R�A�̎擾
//=============================================================================
CScore *CTutorial::GetScore(void)
{
	return m_pScore;
}
//=============================================================================
// �J�����̎擾
//=============================================================================
CCamera *CTutorial::GetCamera(void)
{
	return m_pCamera;
}
