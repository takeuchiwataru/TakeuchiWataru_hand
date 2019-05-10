//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �|���j
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

#include "game.h"
#include "tutorial.h"
#include "player.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CPlayer *CCamera::m_pPlayer = NULL;
CPlayer *CCamera::m_pPlayerT = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_rotDiff = 0.0f;
	m_fLength = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = CAMERASTATE_NORMAL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// �J�����̏���������
//=============================================================================
void CCamera::Init(void)
{

	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRAdd = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_rotDiff = 0.0f;
	m_fLength = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = CAMERASTATE_NORMAL;

	// �r���[�|�[�g�̐ݒ�
	m_ViewPort.X = 0;					// x���W���[
	m_ViewPort.Y = 0;					// y���W��[
	m_ViewPort.Width = SCREEN_WIDTH;	// x���W�E�[
	m_ViewPort.Height = SCREEN_HEIGHT;	// x���W���[
	m_ViewPort.MinZ = 0.0f;				// ��O
	m_ViewPort.MaxZ = 1.0f;				// ��

	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
	{// ���[�h���Q�[�� or �`���[�g���A��
		m_fLength.x = CAMERA_LENGTH_GAME_XZ;
		m_fLength.y = CAMERA_LENGTH_GAME_Y;
		m_fLength.z = CAMERA_LENGTH_GAME_XZ;
	}
	else if (mode == CManager::MODE_TITLE || mode == CManager::MODE_RANKING || mode == CManager::MODE_RESULT)
	{// ���[�h���^�C�g�� or �����L���O or ���U���g
		m_fLength.x = CAMERA_LENGTH_TITLE_XZ;
		m_fLength.y = CAMERA_LENGTH_TITLE_Y;
		m_fLength.z = CAMERA_LENGTH_TITLE_XZ;

		m_posR = CAMERA_CENTER;       // �t�B�[���h�̒��S�ɒ����_�̐ݒ�
	}

	m_posV.x = m_posR.x + sinf(m_rot.y) * m_fLength.x;
	m_posV.y = m_posR.y + cosf(m_rot.y) * m_fLength.y;
	m_posV.z = m_posR.z + cosf(m_rot.y) * m_fLength.z;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TITLE || mode == CManager::MODE_RANKING)
	{// ���[�h���^�C�g����������
		m_rot.y += D3DX_PI / CAMERA_ROT_TITLE;
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
		m_posV.x = m_posR.x + sinf(m_rot.y) * m_fLength.x;
		m_posV.z = m_posR.z + cosf(m_rot.y) * m_fLength.z;
	}

	if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[��
		// �v���C���[�̎擾
		m_pPlayer = CGame::GetPlayer();
		if (m_pPlayer != NULL)
		{
			D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
			D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
			D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();

			// ���_�ݒ�
			m_posVDest.x = (PlayerPos.x + (CAMERA_DEST * PlayerMove.x)) - sinf(m_rot.y) * m_fLength.x;		// �ړI�̎��_��ݒ�
			m_posVDest.y = (PlayerPos.y + m_posVAdd.y) + cosf(0.0f) * m_fLength.y;					// �ړI�̎��_��ݒ�
			m_posVDest.z = (PlayerPos.z + (CAMERA_DEST * PlayerMove.z)) - cosf(m_rot.y) * m_fLength.z;		// �ړI�̎��_��ݒ�

			// �����_�ݒ�
			m_posRDest.x = (PlayerPos.x + (CAMERA_DEST * PlayerMove.x)) + sinf(PlayerRot.y + D3DX_PI) * ((PlayerMove.x * PlayerMove.x) + ((PlayerMove.x * PlayerMove.x)));	// �ړI�̒����_��ݒ�
			m_posRDest.y = (PlayerPos.y + m_posRAdd.y);																										// �ړI�̒����_��ݒ�
			m_posRDest.z = (PlayerPos.z + (CAMERA_DEST * PlayerMove.z)) + cosf(PlayerRot.y + D3DX_PI) * ((PlayerMove.z * PlayerMove.z) + ((PlayerMove.z * PlayerMove.z)));	// �ړI�̒����_��ݒ�

			m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_POSV;	// ���݂̎��_��ݒ�
			m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_POSV;	// ���݂̎��_��ݒ�
			m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_POSV;	// ���݂̎��_��ݒ�

			m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_POSR;	// ���݂̒����_��ݒ�
			m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_POSR;	// ���݂̒����_��ݒ�
			m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_POSR;	// ���݂̒����_��ݒ�

			if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_24) == true || pInputKeyboard->GetPress(DIK_Q) == true)
			{// �E�X�e�B�b�N�����ɓ|������
				m_rot.y -= D3DX_PI / CAMERA_ROT_GAME;
				if (m_rot.y <= -D3DX_PI)
				{
					m_rot.y = D3DX_PI;
				}
			}
			else if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_26) == true || pInputKeyboard->GetPress(DIK_E) == true)
			{// �E�X�e�B�b�N���E�ɓ|������	
				m_rot.y += D3DX_PI / CAMERA_ROT_GAME;
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y = -D3DX_PI;
				}
			}
		}
	}
	if (mode == CManager::MODE_TUTORIAL)
	{// ���[�h���`���[�g���A��
		// �v���C���[�̎擾
		m_pPlayerT = CTutorial::GetPlayer();
		if (m_pPlayerT != NULL)
		{
			D3DXVECTOR3 PlayerPos = m_pPlayerT->GetPos();
			D3DXVECTOR3 PlayerRot = m_pPlayerT->GetRot();
			D3DXVECTOR3 PlayerMove = m_pPlayerT->GetMove();

			// ���_�ݒ�
			m_posVDest.x = (PlayerPos.x + (CAMERA_DEST * PlayerMove.x)) - sinf(m_rot.y) * m_fLength.x;		// �ړI�̎��_��ݒ�
			m_posVDest.y = (PlayerPos.y + m_posVAdd.y) + cosf(0.0f) * m_fLength.y;			// �ړI�̎��_��ݒ�
			m_posVDest.z = (PlayerPos.z + (CAMERA_DEST * PlayerMove.z)) - cosf(m_rot.y) * m_fLength.z;		// �ړI�̎��_��ݒ�

			// �����_�ݒ�
			m_posRDest.x = (PlayerPos.x + (CAMERA_DEST * PlayerMove.x)) + sinf(PlayerRot.y + D3DX_PI) * ((PlayerMove.x * PlayerMove.x) + ((PlayerMove.x * PlayerMove.x)));	// �ړI�̒����_��ݒ�
			m_posRDest.y = (PlayerPos.y + m_posRAdd.y);																										// �ړI�̒����_��ݒ�
			m_posRDest.z = (PlayerPos.z + (CAMERA_DEST * PlayerMove.z)) + cosf(PlayerRot.y + D3DX_PI) * ((PlayerMove.z * PlayerMove.z) + ((PlayerMove.z * PlayerMove.z)));	// �ړI�̒����_��ݒ�

			m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_POSV;	// ���݂̎��_��ݒ�
			m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_POSV;	// ���݂̎��_��ݒ�
			m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_POSV;	// ���݂̎��_��ݒ�

			m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_POSR;	// ���݂̒����_��ݒ�
			m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_POSR;	// ���݂̒����_��ݒ�
			m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_POSR;	// ���݂̒����_��ݒ�

			if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_24) == true || pInputKeyboard->GetPress(DIK_Q) == true)
			{// �E�X�e�B�b�N�����ɓ|������
				m_rot.y -= D3DX_PI / CAMERA_ROT_TITLE;
				if (m_rot.y <= -D3DX_PI)
				{
					m_rot.y = D3DX_PI;
				}
			}
			else if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_26) == true || pInputKeyboard->GetPress(DIK_E) == true)
			{// �E�X�e�B�b�N���E�ɓ|������	
				m_rot.y += D3DX_PI / CAMERA_ROT_GAME;
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y = -D3DX_PI;
				}
			}
		}
	}
	if (mode == CManager::MODE_RESULT)
	{// ���[�h�����U���g
		m_rot.y -= D3DX_PI / CAMERA_ROT_TITLE;
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
		m_posV.x = m_posR.x + sinf(m_rot.y) * m_fLength.x;
		m_posV.z = m_posR.z + cosf(m_rot.y) * m_fLength.z;
	}

	//CDebugProc::Print(1, "PosV:%.1f,%.1f,%.1f\n", m_posV.x, m_posV.y, m_posV.z);
}
//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(void)
{
	CManager manager;	// �C���X�^���X

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�̎擾
	pDevice = manager.GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(ANGLE_VIEW),								// ��p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		CAMERA_SIDE,											// ��O�̈ʒu
		CAMERA_MOTH);											// ���̈ʒu

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
//=============================================================================
// �J�����̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetCamera(void)
{
	return m_rot;
}

