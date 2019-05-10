//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : �|���j
//
//=============================================================================
#include "game.h"
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
#include "pausemat.h"
#include "pauseselect.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CPlayer *CGame::m_pPlayer = NULL;
CScene3D * CGame::m_pScene3D = NULL;
CMeshField * CGame::m_pMeshField = NULL;
CMeshOrbit * CGame::m_pMeshOrbit = NULL;
CScore * CGame::m_pScore = NULL;
CRemain * CGame::m_pRemain = NULL;
CCamera * CGame::m_pCamera = NULL;
CLight * CGame::m_pLight = NULL;
CTimer * CGame::m_pTimer = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_nTime = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}
//=============================================================================
// ����������
//=============================================================================
void CGame::Init(void)
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

	// �i���o�[�e�N�X�`���̓ǂݍ���
	CNumber::Load();

	// �t���[���̓ǂݍ���
	CFrame::Load();

	// �摜�̓ǂݍ���
	CImage::Load();

	// �p�[�e�B�N���̓ǂݍ���
	CParticle::Load();

	// �|�[�Y�䎆�ǂݍ���
	CPauseMat::Load();

	// �|�[�Y3���ڂ̓ǂݍ���
	CPauseSelect::Load();

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

	 //�G�̐���
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, -300.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(1200.0f, 0.0f, -1200.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(1100.0f, 0.0f, -200.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, -800.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, -500.0f), CEnemy::ENEMYTYPE_002);
	CEnemy::Create(D3DXVECTOR3(1000.0f, 0.0f, -1000.0f), CEnemy::ENEMYTYPE_004);
	CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, -150.0f), CEnemy::ENEMYTYPE_003);
	CEnemy::Create(D3DXVECTOR3(1300.0f, 0.0f, -1000.0f), CEnemy::ENEMYTYPE_005);
	CEnemy::Create(D3DXVECTOR3(1300.0f, 0.0f, -600.0f), CEnemy::ENEMYTYPE_004);
	CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, -1250.0f), CEnemy::ENEMYTYPE_005);

	// ���̐���
	CGrass::Create(D3DXVECTOR3(300.0f, 0.0f, -300.0f),CGrass::GRASSTYPE_001);
	CGrass::Create(D3DXVECTOR3(1200.0f, 0.0f, -1200.0f), CGrass::GRASSTYPE_001);
	CGrass::Create(D3DXVECTOR3(300.0f, 0.0f, -800.0f), CGrass::GRASSTYPE_001);
	CGrass::Create(D3DXVECTOR3(1100.0f, 0.0f, -200.0f), CGrass::GRASSTYPE_001);

	// ���Ȃ���
	CGrass::Create(D3DXVECTOR3(550.0f, 0.0f, -400.0f),CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(850.0f, 0.0f, -300.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(800.0f, 0.0f, -1150.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(1200.0f, 0.0f, -450.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(350.0f, 0.0f, -630.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(650.0f, 0.0f, -730.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(1100.0f, 0.0f, -830.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(650.0f, 0.0f, -1000.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(350.0f, 0.0f, -1100.0f), CGrass::GRASSTYPE_000);

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

	// UI
	CImage::Create(D3DXVECTOR3(820, 640, 0), CImage::IMAGE_NOKORI);
	CImage::Create(D3DXVECTOR3(1010, 640, 0), CImage::IMAGE_REMAIN);
	if (m_pRemain == NULL)
	{// �T���̎c��̐��̐���
		m_pRemain = CRemain::Create(D3DXVECTOR3(1230, 650, 0));
	}

	// �^�C�}�[�t���[���̐���
	CFrame::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -10, 80, 0), CFrame::FRAMETYPE_TIMER);

	if (m_pTimer == NULL)
	{// �^�C�}�[�̐���
		m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 80, 0));
	}
}
//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
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

	// �摜�̔j��
	CImage::Unload();

	// �p�[�e�B�N���̃e�N�X�`���j��
	CParticle::Unload();

	// �|�[�Y�䎆�̔j��
	CPauseMat::Unload();

	// �|�[�Y3���ڂ̔j��
	CPauseSelect::Unload();

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
void CGame::Update(void)
{
	m_nTime++;
	// �f�o�b�O�\��
	CDebugProc::Print(1, "�Q�[��\n");

	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h�̎擾
	CFade *pFade = CManager::GetFade();

	/*�f�o�b�O�p*/
	//if (m_nTime % 60 == 0)
	//{// ��莞�Ԃ�������
	//	pFade->Set(CManager::MODE_GAME, pFade->FADE_OUT);
	//}

	if (m_pCamera != NULL)
	{// �J�����̍X�V����
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ���C�g�̍X�V����
		m_pLight->Update();
	}

#ifdef _DEBUG
	// ���C���[�t���[��
	if (pInputKeyboard->GetPress(DIK_F1) == true)
	{// F1����������ON
		pDevice->SetRenderState(D3DRS_FILLMODE, 2);
	}
	else if (pInputKeyboard->GetPress(DIK_F2) == true)
	{// F2����������OFF
		pDevice->SetRenderState(D3DRS_FILLMODE, 0);
	}
	//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{// �t�F�[�h
	//	pFade->Set(CManager::MODE_RESULT, pFade->FADE_OUT);
	//}

#endif

}
//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	if (m_pCamera != NULL)
	{// �J������ݒu
		m_pCamera->SetCamera();
	}
}
//=============================================================================
// Scene3D�̎擾
//=============================================================================
CScene3D *CGame::GetScene3D(void)
{
	return m_pScene3D;
}
//=============================================================================
// ���b�V���t�B�[���h�̎擾
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}
//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}
//=============================================================================
// �c�@�̎擾
//=============================================================================
CRemain *CGame::GetRemain(void)
{
	return m_pRemain;
}
//=============================================================================
// �X�R�A�̎擾
//=============================================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}
//=============================================================================
// �J�����̎擾
//=============================================================================
CCamera *CGame::GetCamera(void)
{
	return m_pCamera;
}
//=============================================================================
// �^�C�}�[�̎擾
//=============================================================================
CTimer *CGame::GetTimer(void)
{
	return m_pTimer;
}




