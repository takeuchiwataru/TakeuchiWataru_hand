//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : �|���j
//
//=============================================================================
#include "title.h"
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
#include "logo.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CMeshField * CTitle::m_pMeshField = NULL;
CCamera * CTitle::m_pCamera = NULL;
CLight * CTitle::m_pLight = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	m_nTime = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
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

	// �^�C�g�����S�̓ǂݍ���
	CTitleLogo::Load();

	// �v���X�G���^�[�̓ǂݍ���
	CPressEnter::Load();

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
	CSky::Create(D3DXVECTOR3(750.0f, 500.0f, -750.0f));

	//======================
	// UI
	//======================

	// �^�C�g�����S�N���X�̐���
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	// �v���X�G���^�[�̐���
	CPressEnter::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250, 0.0f), CPressEnter::PRESSENTER_TITLE);
	CPressEnter::DispPressEnter();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	//======================
	// ���f���̔j��
	//======================

	// �I�u�W�F�N�g�̔j��
	CObject::UnloadX();

	//======================
	// �e�N�X�`���̔j��
	//======================

	// �^�C�g�����S�e�N�X�`���j��
	CTitleLogo::Unload();

	// �v���X�G���^�[�e�N�X�`���j��
	CPressEnter::Unload();

	// �S�ĉ��
	CScene::ReleseAll();

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
void CTitle::Update(void)
{
	m_nTime++;

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// �t�F�[�h�̎擾
	CFade *pFade = CManager::GetFade();

	if (pInputJoyPad != NULL)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_1) == true ||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_2) == true ||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true ||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true )
		{// �Ώۂ̃{�^���������ꂽ��
			pFade->Set(CManager::MODE_TUTORIAL, pFade->FADE_OUT);
		}
	}
	if (m_nTime % 300 == 0)
	{// ��莞�Ԃ�������
		pFade->Set(CManager::MODE_RANKING, pFade->FADE_OUT);
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
	//	CManager::SetMode(CManager::MODE_TUTORIAL);
	//}

	// �f�o�b�O�\��
	CDebugProc::Print(1, "�^�C�g��\n");
}
//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	if (m_pCamera != NULL)
	{// �J������ݒu
		m_pCamera->SetCamera();
	}
}
