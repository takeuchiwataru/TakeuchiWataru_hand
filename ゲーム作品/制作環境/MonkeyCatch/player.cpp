//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �|���j
//
//=============================================================================
#include "player.h"
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"

#include "scene3D.h"
#include "meshfield.h"
#include "meshorbit.h"
#include "object.h"
#include "enemy.h"
#include "particle.h"
#include "grass.h"

#include "game.h"
#include "tutorial.h"
#include "scene.h"
#include "sound.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
LPD3DXMESH CPlayer::m_apMesh[MAX_PLAYER_MODEL] = {};
LPD3DXBUFFER CPlayer::m_apBuffMat[MAX_PLAYER_MODEL] = {};
DWORD CPlayer::m_anNumMat[MAX_PLAYER_MODEL] = {};
CMeshOrbit * CPlayer::m_pMeshOrbit = NULL;
CObject * CPlayer::m_pObject = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = PLAYERSTATE_NONE;

	m_pKeyInfo = NULL;
	m_nKey = 0;
	m_nCouterMotion = 0;
	m_nNumParts = 0;
	m_StateSave = 0;

	m_nAttackCounter = 0;
	m_nJumpCounter = 0;

	for (int nCnt = 0; nCnt < MAX_PLAYER_MODEL; nCnt++)
	{
		m_apModel[nCnt] = NULL;
		m_aIndexParent[nCnt] = 0;
		m_aPosOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_aPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aRot[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// ���[�V�����̓ǂݍ���
	LoadMotion();

	// �I�u�W�F�N�g�̃^�C�v�̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	// ���̏�����
	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = PLAYERSTATE_NONE;
	m_bJump = false;
	m_bLand = false;
	m_fDest = 0.0f;
	m_fDiff = 0.0f;
	m_bAttack = false;
	m_bHit = false;
	m_nAttackCounter = 0;
	m_nJumpCounter = 0;

	m_nKey = 0;

	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);

	int nNumVtex;					// ���_��
	DWORD sizeFvF;					// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		// ���_�����擾
		nNumVtex = m_apMesh[nCntModel]->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFvF = D3DXGetFVFVertexSize(m_apMesh[nCntModel]->GetFVF());

		// ���_�o�b�t�@�����b�N
		m_apMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
			if (m_VtxMin.x >= vtx.x)
			{
				m_VtxMin.x = vtx.x;
			}
			if (m_VtxMin.y >= vtx.y)
			{
				m_VtxMin.y = vtx.y;
			}
			if (m_VtxMin.z >= vtx.z)
			{
				m_VtxMin.z = vtx.z;
			}

			if (m_VtxMax.x <= vtx.x)
			{
				m_VtxMax.x = vtx.x;
			}
			if (m_VtxMax.y <= vtx.y)
			{
				m_VtxMax.y = vtx.y;
			}
			if (m_VtxMax.z <= vtx.z)
			{
				m_VtxMax.z = vtx.z;
			}
			pVtxBuff += sizeFvF;
		}
		// ���_�o�b�t�@���A�����b�N
		m_apMesh[nCntModel]->UnlockVertexBuffer();
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	if (m_pMeshOrbit != NULL)
	{// ���b�V���I�[�r�b�g��NULL����Ȃ�������NULL�ɂ���
		m_pMeshOrbit = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	if (mode == CManager::MODE_TUTORIAL)
	{// ���[�h���`���[�g���A��
		UpdateTutorial();
	}

	if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[��
		// �J�����̎擾
		CCamera *pCamera = CGame::GetCamera();
		D3DXVECTOR3 rot = pCamera->GetCamera();

		// �L�[�{�[�h�̎擾
		CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

		// �W���C�p�b�h�̎擾
		CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

		// �ړ�����
		Move();

		m_pos.y -= m_move.y;
		m_move.y += PLAYER_GRAVITY;		// �d�͉��Z

		// ���b�V���t�B�[���h�̎擾
		CMeshField *pMeshField = CGame::GetMeshField();
		float fHight = pMeshField->GetHeight(m_pos);

		if (m_pos.y < fHight)
		{
			m_pos.y = fHight;
			m_bJump = false;
			m_bLand = false;

			if (pInputKeyboard->GetTrigger(DIK_J) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true)
			{// �L�[�{�[�h��J or A�{�^���ŃW�����v
				m_bJump = true;
				m_move.y = -PLAYER_JUMP;
				m_nKey = 0;
				m_nCouterMotion = 0;
				pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);
			}
		}
		if (m_bAttack == false)
		{// �U��������Ȃ�������
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
			{// ENTER or B�{�^���Œ���
				m_bAttack = true;
				m_nKey = 0;
				m_nCouterMotion = 0;
				pSound->PlaySound(CSound::SOUND_LABEL_SE_MUSIAMI);

				if (m_pMeshOrbit == NULL)
				{// ���b�V���I�[�r�b�g�̐���
					m_pMeshOrbit = CMeshOrbit::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
				}
				// ����Ƀ��b�V���I�[�r�b�g��ݒ�
				m_pMeshOrbit->SetMtxParent(&m_apModel[10]->GetMtxWorld());
			}
		}

		if (m_move.x < PLAYER_MOVESTAND && m_move.x > -PLAYER_MOVESTAND && m_move.z < PLAYER_MOVESTAND && m_move.z > -PLAYER_MOVESTAND && m_bJump == false)
		{// �j���[�g����
			if (m_state == PLAYERSTATE_MOVE)
			{// �ړ����Ă鎞
				m_nKey = 0;
				m_nCouterMotion = 0;
			}
			m_state = PLAYERSTATE_NORMAL;
		}
		if (m_move.x >= PLAYER_MOVESTAND || m_move.x <= -PLAYER_MOVESTAND || m_move.z >= PLAYER_MOVESTAND || m_move.z <= -PLAYER_MOVESTAND && m_bJump == false)
		{// �ړ�
			m_state = PLAYERSTATE_MOVE;

			for (int nCntParticle = 0; nCntParticle < P_MAX_PARTICLE; nCntParticle++)
			{// �p�[�e�B�N���̐���
				CParticle::Create(P_PARTICLE_CREATEPOS, D3DXVECTOR3(sinf(P_PARTICLE_AROUND) * P_PARTICLE_RAND, cosf(P_PARTICLE_AROUND) * P_PARTICLE_RAND, 0.0f),
					P_PARTICLE_LIFE, P_PARTICLE_RADIUS, CParticle::PARTICLETYPE_001);
			}
		}
		if (m_bJump == true)
		{// �W�����v
			m_state = PLAYERSTATE_JUMP;
		}
		if (m_bLand == true)
		{// ���n
			m_state = PLAYERSTATE_LANDING;
		}
		if (m_bAttack == true)
		{// ���ԐU��
			m_nAttackCounter++;
			m_state = PLAYERSTATE_MUSIAMI;

			if (m_nAttackCounter >= ATTACK_COUNTER)
			{// ��莞�Ԃ�������
				m_bAttack = false;
				m_nAttackCounter = 0;
			}
		}

		// �ʒu�X�V
		m_pos.x += m_move.x;
		m_pos.z += m_move.z;

		// ����
		m_move.x += (0.0f - m_move.x) * 0.7f;
		m_move.z += (0.0f - m_move.z) * 0.7f;

		// ���[�V�����̍X�V
		UpdateMotion();

		// �����蔻��S��
		CollisionAll();

		// �}�b�v�����蔻��
		MapCollision();
	}

	// �f�o�b�O�\��
	CDebugProc::Print(1, "�v���C���[�̑��� : [��,��,��,��  �ړ�][ENTER ����][J �W�����v]\n");
	CDebugProc::Print(1, "�v���C���[�̈ʒu : X %.1f, Y %.1f, Z %.1f\n", m_pos.x, m_pos.y, m_pos.z);
}
//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::UpdateTutorial(void)
{
	// �J�����̎擾
	CCamera *pCamera = CTutorial::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetCamera();

	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// �ߋ��̈ʒu���擾
	m_posold = m_pos;

	//===================
	// �L�[�{�[�h
	//===================
	if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_A) == true)
	{// �� or A����������
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//����ړ�
			m_move.x -= sinf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//�����ړ�
			m_move.x -= sinf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.25f);
		}
		else
		{// ���ړ�
			m_move.x -= sinf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{// �� or D����������
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//�E��ړ�
			m_move.x -= sinf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//�E���ړ�
			m_move.x -= sinf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.25f);
		}
		else
		{// �E�ړ�
			m_move.x -= sinf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
	{// �� or W����������
		m_move.x += sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z += cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 1.0f);
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
	{// �� or S����������
		m_move.x -= sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z -= cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 0.0f);
	}

	//===================
	// �R���g���[���[
	//===================
	if (pInputJoyPad != NULL)
	{
		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_20) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_21) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_22) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_23) == true)
		{// ���X�e�B�b�N
			m_move.x -= sinf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;

			m_fDest = rot.y - pInputJoyPad->GetLeftAxiz();
		}

		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_28) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_29) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_30) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_31) == true)
		{// �\���L�[
			m_move.x += sinf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;
			m_move.z += cosf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;

			m_fDest = rot.y + pInputJoyPad->GetRadian() - D3DX_PI;
		}
	}

	// �p�x�̐ݒ�
	SetAngle();

	m_pos.y -= m_move.y;
	m_move.y += PLAYER_GRAVITY;		// �d�͉��Z

	// ���b�V���t�B�[���h�̎擾
	CMeshField *pMeshField = CTutorial::GetMeshField();
	float fHight = pMeshField->GetHeight(m_pos);

	if (m_pos.y < fHight)
	{
		m_pos.y = fHight;
		m_bJump = false;
		m_bLand = false;

		if (pInputKeyboard->GetTrigger(DIK_J) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true)
		{// �L�[�{�[�h��J or A�{�^���ŃW�����v
			m_bJump = true;
			m_move.y = -PLAYER_JUMP;
			m_nKey = 0;
			m_nCouterMotion = 0;
			pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);
		}
	}
	if (m_bAttack == false)
	{// �U��������Ȃ�������
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
		{// ENTER or B�{�^���Œ���
			m_bAttack = true;
			m_nKey = 0;
			m_nCouterMotion = 0;
			pSound->PlaySound(CSound::SOUND_LABEL_SE_MUSIAMI);

			if (m_pMeshOrbit == NULL)
			{// ���b�V���I�[�r�b�g�̐���
				m_pMeshOrbit = CMeshOrbit::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
			}
			// ����Ƀ��b�V���I�[�r�b�g��ݒ�
			m_pMeshOrbit->SetMtxParent(&m_apModel[10]->GetMtxWorld());
		}
	}

	if (m_move.x < PLAYER_MOVESTAND && m_move.x > -PLAYER_MOVESTAND && m_move.z < PLAYER_MOVESTAND && m_move.z > -PLAYER_MOVESTAND && m_bJump == false)
	{// �j���[�g����
		if (m_state == PLAYERSTATE_MOVE)
		{// �ړ����Ă鎞
			m_nKey = 0;
			m_nCouterMotion = 0;
		}

		m_state = PLAYERSTATE_NORMAL;
	}
	if (m_move.x >= PLAYER_MOVESTAND || m_move.x <= -PLAYER_MOVESTAND || m_move.z >= PLAYER_MOVESTAND || m_move.z <= -PLAYER_MOVESTAND && m_bJump == false)
	{// �ړ�
		m_state = PLAYERSTATE_MOVE;
	}
	if (m_bJump == true)
	{// �W�����v
		m_state = PLAYERSTATE_JUMP;
	}
	if (m_bLand == true)
	{// ���n
		m_state = PLAYERSTATE_LANDING;
	}
	if (m_bAttack == true)
	{// ���ԐU��
		m_nAttackCounter++;
		m_state = PLAYERSTATE_MUSIAMI;

		if (m_nAttackCounter >= ATTACK_COUNTER)
		{// ��莞�Ԃ�������
			m_bAttack = false;
			m_nAttackCounter = 0;
		}
	}

	// �ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	// ����
	m_move.x += (0.0f - m_move.x) * 0.7f;
	m_move.z += (0.0f - m_move.z) * 0.7f;

	// ���[�V�����̍X�V
	UpdateMotion();

	// �����蔻��S��
	CollisionAll();

	// �}�b�v�����蔻��
	MapCollision();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̕`�揈��
	D3DXMATRIX	mtxShadow;
	D3DXPLANE	planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;

	// ���C�g�𖳌��ɂ���
	pDevice->LightEnable(0, FALSE);
	pDevice->LightEnable(1, FALSE);
	pDevice->LightEnable(2, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = PLAYER_VECLIGHT;
	pos = PLAYER_POS;
	normal = PLAYER_NORMAL;

	// ���ʂ��쐬
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ���ʂɎˉe����}�g���b�N�X���쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ���[���h�}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}
	// ���̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}
}
//============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	// �J�����̎擾
	CCamera *pCamera = CGame::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetCamera();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// �ߋ��̈ʒu���擾
	m_posold = m_pos;

	//===================
	// �L�[�{�[�h
	//===================
	if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_A) == true)
	{// �� or A����������
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//����ړ�
			m_move.x -= sinf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//�����ړ�
			m_move.x -= sinf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.25f);
		}
		else
		{// ���ړ�
			m_move.x -= sinf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{// �� or D����������
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//�E��ړ�
			m_move.x -= sinf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//�E���ړ�
			m_move.x -= sinf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.25f);
		}
		else
		{// �E�ړ�
			m_move.x -= sinf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
	{// �� or W����������
		m_move.x += sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z += cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 1.0f);
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
	{// �� or S����������
		m_move.x -= sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z -= cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 0.0f);
	}

	//===================
	// �R���g���[���[
	//===================
	if (pInputJoyPad != NULL)
	{
		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_20) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_21) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_22) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_23) == true)
		{// ���X�e�B�b�N
			m_move.x -= sinf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;

			m_fDest = rot.y - pInputJoyPad->GetLeftAxiz();
		}

		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_28) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_29) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_30) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_31) == true)
		{// �\���L�[
			m_move.x += sinf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;
			m_move.z += cosf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;

			m_fDest = rot.y + pInputJoyPad->GetRadian() - D3DX_PI;
		}
	}

	// �p�x�̐ݒ�
	SetAngle();
}
//=============================================================================
// ���[�V�����e�L�X�g�̓ǂݍ���
//=============================================================================
void CPlayer::LoadMotion(void)
{
	FILE * pFile;
	char *pStrCur;						// ������̐擪�|�C���^
	char aLine[MAX_CHAR];					// ������ǂݍ��ݗp
	char aStr[MAX_CHAR];						// �����񔲂��o���p
	int nWord;
	int nSpace;
	int nIndex;							// �C���f�b�N�X
	int nLoop;							// ���[�v
	int nCntMotion = 0;					// ���[�V����

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �t�@�C���I�[�v��
	pFile = fopen(PLAYER_FILE, "r");

	if (pFile != NULL)
	{// �t�@�C�����J������
		while (1)
		{
			pStrCur = ReadLine(pFile, &aLine[0]);
			pStrCur = GetLineTop(&aLine[0]);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], NUM_MODEL, strlen(NUM_MODEL)) == 0)
			{// ���f����
				pStrCur += strlen(NUM_MODEL);	// ���o��

				strcpy(&aStr[0], pStrCur);		// �Ώۂ̕����񔲂��o��

				m_nNumParts = atoi(pStrCur);	// �������l�ɕϊ�

				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{// ���f�������J��Ԃ�
					pStrCur = ReadLine(pFile, &aLine[0]);
					pStrCur = GetLineTop(&aLine[0]);
					strcpy(&aStr[0], pStrCur);

					if (memcmp(&aStr[0], MODEL_FILENAME, strlen(MODEL_FILENAME)) == 0)
					{// ���f���t�@�C��
						pStrCur += strlen(MODEL_FILENAME);	// ���o��
						strcpy(&aStr[0], pStrCur);

						// �R�����g����
						nSpace = PopString(pStrCur, &aStr[0]);
						strcpy(&aStr[0], pStrCur);
						aStr[nSpace - 1] = '\0';

						strcpy(&m_aFilenameModel[nCntModel][0], aStr);	// �Ώۂ̕����񔲂��o��

						// X�t�@�C���̓ǂݍ���
						D3DXLoadMeshFromX(&m_aFilenameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_apBuffMat[nCntModel],
							NULL,
							&m_anNumMat[nCntModel],
							&m_apMesh[nCntModel]);
					}
				}
			}
			if (memcmp(&aStr[0], CHARACTERSET, strlen(CHARACTERSET)) == 0)
			{// �L�����N�^�[���

			 // 1�s�ǂݍ���
				pStrCur = ReadLine(pFile, &aLine[0]);
				pStrCur = GetLineTop(&aLine[0]);

				strcpy(&aStr[0], pStrCur);

				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{
					if (memcmp(&aStr[0], NUM_PARTS, strlen(NUM_PARTS)) == 0)
					{// �p�[�c��
						pStrCur += strlen(NUM_PARTS);	// ���o��

						strcpy(&aStr[0], pStrCur);	// �Ώۂ̕����񔲂��o��

						m_nNumParts = atoi(pStrCur);	// �������l�ɕϊ�	

						// 1�s�ǂݍ���
						pStrCur = ReadLine(pFile, &aLine[0]);
						pStrCur = GetLineTop(&aLine[0]);

						strcpy(&aStr[0], pStrCur);
					}

					if (memcmp(&aStr[0], PARTSSET, strlen(PARTSSET)) == 0)
					{// �p�[�c���

					 // 1�s�ǂݍ���
						pStrCur = ReadLine(pFile, &aLine[0]);
						pStrCur = GetLineTop(&aLine[0]);

						strcpy(&aStr[0], pStrCur);

						if (memcmp(&aStr[0], INDEX, strlen(INDEX)) == 0)
						{// �C���f�b�N�X

							pStrCur += strlen(INDEX);	// ���o��

							strcpy(&aStr[0], pStrCur);	// �Ώۂ̕����񔲂��o��

							nIndex = atoi(pStrCur);	// �������l�ɕϊ�

							// 1�s�ǂݍ���
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], PARENT, strlen(PARENT)) == 0)
						{// �e�q�֌W
							pStrCur += strlen(PARENT);	// ���o��

							strcpy(&aStr[0], pStrCur);	// �Ώۂ̕����񔲂��o��

							m_aIndexParent[nIndex] = atoi(pStrCur);	// �������l�ɕϊ�

							// 1�s�ǂݍ���
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], POS, strlen(POS)) == 0)
						{// �ʒu
							pStrCur += strlen(POS);		// ���o��

							m_aPos[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							m_aPos[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							m_aPos[nIndex].z = (float)atof(pStrCur);	// Z

							// 1�s�ǂݍ���
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], ROT, strlen(ROT)) == 0)
						{// ����
							pStrCur += strlen(ROT);		// ���o��

							m_aRot[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							m_aRot[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							m_aRot[nIndex].z = (float)atof(pStrCur);	// Z

							// 1�s�ǂݍ���
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}
						if (memcmp(&aStr[0], END_PARTSSET, strlen(END_PARTSSET)) == 0)
						{
							// 1�s�ǂݍ���
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (m_aIndexParent[nIndex] == -1)
						{// �e
							m_apModel[0] = CModel::Create(D3DXVECTOR3(m_pos.x + m_aPos[nIndex].x, m_pos.y + m_aPos[nIndex].y, m_aPos[nIndex].z));
							m_apModel[0]->BindModel(m_apMesh[nIndex], m_apBuffMat[nIndex], m_anNumMat[nIndex]);
							m_apModel[0]->SetParent(NULL);			// ��

							// �I�t�Z�b�g�ɑ��
							m_aPosOffset[0] = m_apModel[0]->GetPos();
						}
						else if (m_aIndexParent[nIndex] > -1)
						{// �q
							m_apModel[nIndex] = CModel::Create(D3DXVECTOR3(m_aPos[nIndex].x, m_aPos[nIndex].y, m_aPos[nIndex].z));
							m_apModel[nIndex]->BindModel(m_apMesh[nIndex], m_apBuffMat[nIndex], m_anNumMat[nIndex]);
							m_apModel[nIndex]->SetParent(m_apModel[m_aIndexParent[nIndex]]);

							// �I�t�Z�b�g�ɑ��
							m_aPosOffset[nIndex] = m_apModel[nIndex]->GetPos();
						}
					}
				}
			}
			if (memcmp(&aStr[0], END_CHARACTERSET, strlen(END_CHARACTERSET)) == 0)
			{
				// 1�s�ǂݍ���
				pStrCur = ReadLine(pFile, &aLine[0]);
				pStrCur = GetLineTop(&aLine[0]);

				strcpy(&aStr[0], pStrCur);
			}
			if (memcmp(&aStr[0], MOTIONSET, strlen(MOTIONSET)) == 0)
			{// ���[�V�������
				while (1)
				{
					// 1�s�ǂݍ���
					pStrCur = ReadLine(pFile, &aLine[0]);
					pStrCur = GetLineTop(&aLine[0]);

					strcpy(&aStr[0], pStrCur);

					if (memcmp(&aStr[0], LOOP, strlen(LOOP)) == 0)
					{// ���[�v���邩�ǂ���
						pStrCur += strlen(LOOP);	// ���o��

						strcpy(&aStr[0], pStrCur);	// �Ώۂ̕����񔲂��o��

						nLoop = atoi(pStrCur);	// �������l�ɕϊ�	

						if (nLoop == 1)
						{// 1�������烋�[�v����
							m_aMotionInfo[nCntMotion].bLoop = true;
						}
						else if (nLoop == 0)
						{// 0�������烋�[�v���Ȃ�
							m_aMotionInfo[nCntMotion].bLoop = false;
						}
					}
					if (memcmp(&aStr[0], NUM_KEY, strlen(NUM_KEY)) == 0)
					{// �L�[��
						pStrCur += strlen(NUM_KEY);	// ���o��

						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrCur);

						// 1�s�ǂݍ���
						pStrCur = ReadLine(pFile, &aLine[0]);
						pStrCur = GetLineTop(&aLine[0]);

						strcpy(&aStr[0], pStrCur);

						for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey; nCntKey++)
						{// �L�[�����J��Ԃ�
							if (memcmp(&aStr[0], KEYSET, strlen(KEYSET)) == 0)
							{// �L�[���n��

							 // 1�s�ǂݍ���
								pStrCur = ReadLine(pFile, &aLine[0]);
								pStrCur = GetLineTop(&aLine[0]);

								strcpy(&aStr[0], pStrCur);

								if (memcmp(&aStr[0], FRAME, strlen(FRAME)) == 0)
								{// �t���[����
									pStrCur += strlen(FRAME);	// ���o��

									m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(pStrCur);
								}

								// 1�s�ǂݍ���
								pStrCur = ReadLine(pFile, &aLine[0]);
								pStrCur = GetLineTop(&aLine[0]);

								strcpy(&aStr[0], pStrCur);

								for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
								{// ���f�������̌J��Ԃ�
									if (memcmp(&aStr[0], KEY, strlen(KEY)) == 0)
									{// �L�[���

									 // 1�s�ǂݍ���
										pStrCur = ReadLine(pFile, &aLine[0]);
										pStrCur = GetLineTop(&aLine[0]);

										strcpy(&aStr[0], pStrCur);

										if (memcmp(&aStr[0], POS, strlen(POS)) == 0)
										{// �ʒu
											pStrCur += strlen(POS);		// ���o��

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosX = (float)atof(pStrCur);	// X

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// ���܂Ői�߂�

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosY = (float)atof(pStrCur);	// Y

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// ���܂Ői�߂�

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosZ = (float)atof(pStrCur);	// Z

																																		// 1�s�ǂݍ���
											pStrCur = ReadLine(pFile, &aLine[0]);
											pStrCur = GetLineTop(&aLine[0]);

											strcpy(&aStr[0], pStrCur);
										}

										if (memcmp(&aStr[0], ROT, strlen(ROT)) == 0)
										{// ����
											pStrCur += strlen(ROT);		// ���o��

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotX = (float)atof(pStrCur);	// X

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// ���܂Ői�߂�

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotY = (float)atof(pStrCur);	// Y

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// ���܂Ői�߂�

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotZ = (float)atof(pStrCur);	// Z

											// 1�s�ǂݍ���
											pStrCur = ReadLine(pFile, &aLine[0]);
											pStrCur = GetLineTop(&aLine[0]);

											strcpy(&aStr[0], pStrCur);
										}
										if (memcmp(&aStr[0], END_KEY, strlen(END_KEY)) == 0)
										{
											// 1�s�ǂݍ���
											pStrCur = ReadLine(pFile, &aLine[0]);
											pStrCur = GetLineTop(&aLine[0]);

											strcpy(&aStr[0], pStrCur);
										}
									}
								}
							}
							if (memcmp(&aStr[0], END_KEYSET, strlen(END_KEYSET)) == 0)
							{// �L�[���I���
							 // 1�s�ǂݍ���
								pStrCur = ReadLine(pFile, &aLine[0]);
								pStrCur = GetLineTop(&aLine[0]);

								strcpy(&aStr[0], pStrCur);
							}
						}
					}
					if (memcmp(&aStr[0], END_MOTIONSET, strlen(END_MOTIONSET)) == 0)
					{
						m_pKeyInfo = &m_aMotionInfo[nCntMotion].aKeyInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			if (memcmp(&aStr[0], "END_SCRIPT", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
}
//=============================================================================
// ���[�V�����̍X�V
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	m_StateSave = m_state - 1;

	P_KEY *pKey, *pKeyNext;				// ���݂̃L�[,���̃L�[
	float fRateMotion;					// ���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Δ���Z�o
	float fDiffMotion;					// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rotMotion;

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// ���f���̃p�[�c���J��Ԃ�

		if (m_apModel[nCntModel] != NULL)
		{
			// ���݂̃L�[���擾
			pKey = &m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].aKey[nCntModel];

			// ���̃L�[���擾
			pKeyNext = &m_aMotionInfo[m_StateSave].aKeyInfo[(m_nKey + 1) % m_aMotionInfo[m_StateSave].nNumKey].aKey[nCntModel];

			// ���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Βl���Z�o
			fRateMotion = (float)(m_nCouterMotion) / (float)m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame;

			//==============
			//�@POS
			//==============

			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
			fDiffMotion = pKeyNext->fPosX - pKey->fPosX;
			// ���Βl�ƍ������g���Ċe�v�f�̒l���Z�o
			posMotion.x = pKey->fPosX + (fDiffMotion * fRateMotion);

			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
			fDiffMotion = pKeyNext->fPosY - pKey->fPosY;
			// ���Βl�ƍ������g���Ċe�v�f�̒l���Z�o
			posMotion.y = pKey->fPosY + (fDiffMotion * fRateMotion);

			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
			fDiffMotion = pKeyNext->fPosZ - pKey->fPosZ;
			// ���Βl�ƍ������g���Ċe�v�f�̒l���Z�o
			posMotion.z = pKey->fPosZ + (fDiffMotion * fRateMotion);

			//==============
			//�@ROT
			//==============

			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
			fDiffMotion = pKeyNext->fRotX - pKey->fRotX;
			// ���Βl�ƍ������g���Ċe�v�f�̒l���Z�o
			rotMotion.x = pKey->fRotX + (fDiffMotion * fRateMotion);

			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
			fDiffMotion = pKeyNext->fRotY - pKey->fRotY;
			// ���Βl�ƍ������g���Ċe�v�f�̒l���Z�o
			rotMotion.y = pKey->fRotY + (fDiffMotion * fRateMotion);

			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍�����l���Z�o
			fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;
			// ���Βl�ƍ������g���Ċe�v�f�̒l���Z�o
			rotMotion.z = pKey->fRotZ + (fDiffMotion * fRateMotion);

			// �ʒu
			m_apModel[nCntModel]->SetPos(m_aPosOffset[nCntModel] + posMotion);

			// ����
			m_apModel[nCntModel]->SetRot(rotMotion);
		}
	}
	m_nCouterMotion++;

	if (m_aMotionInfo[m_StateSave].bLoop == true)
	{//�@���[�v����
		if (m_nCouterMotion >= m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame)
		{
			if (m_nKey >= m_aMotionInfo[m_StateSave].nNumKey - 1)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCouterMotion = 0;
		}
	}
	else
	{// ���[�v���Ȃ�
		if (m_nCouterMotion > m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame)
		{
			if (m_nKey < m_aMotionInfo[m_StateSave].nNumKey - 1)
			{
				m_nKey++;
			}
			else if (m_nKey >= m_aMotionInfo[m_StateSave].nNumKey)
			{
				m_nKey = 0;
			}
			m_nCouterMotion = 0;
		}
	}

	// �f�o�b�O�\��
	CDebugProc::Print(1, " %d / %d\n", m_nCouterMotion, m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame);
	CDebugProc::Print(1, "���݂̃L�[ :  %d\n", m_nKey);
	CDebugProc::Print(1, "�v���C���[�̏�� : %d\n\n", m_state);
}
//=============================================================================
// �����ȍs����
//=============================================================================
char *CPlayer::ReadLine(FILE * pFile, char *pDst)
{
	bool bChar = true;
	while (bChar == true)
	{
		fgets(pDst, MAX_CHAR, pFile);

		if (pDst[0] == '#')
		{// �R�����g�������ꍇ

		}
		else if (pDst[0] == '\n')
		{// ���s�������ꍇ

		}
		else if (pDst[0] == '\t')
		{
			for (int nCntChar = 0; nCntChar < MAX_CHAR; nCntChar++)
			{
				if (pDst[nCntChar + 1] == '#')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\n')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\t')
				{

				}
				else if (pDst[nCntChar + 1] == ' ')
				{

				}
				else
				{
					bChar = false;
					break;
				}
			}
		}
		else if (pDst[0] == ' ')
		{
			for (int nCntChar = 0; nCntChar < MAX_CHAR; nCntChar++)
			{
				if (pDst[nCntChar + 1] == '#')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\n')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\t')
				{

				}
				else if (pDst[nCntChar + 1] == ' ')
				{

				}
				else
				{
					bChar = false;
					break;
				}
			}
		}
		else
		{
			bChar = false;
		}
	}
	return pDst;
}
//=============================================================================
// tab�Ƃ����΂�
//=============================================================================
char *CPlayer::GetLineTop(char *pSrc)
{
	while (1)
	{
		if (memcmp(&pSrc[0], "\t", strlen("\t")) == 0)
		{// �^�u�������ꍇ
			pSrc += strlen("\t");
		}
		else if (memcmp(&pSrc[0], " ", strlen(" ")) == 0)
		{// �X�y�[�X�������ꍇ
			pSrc += strlen(" ");
		}
		else
		{
			break;
		}
	}
	return pSrc;
}
//=============================================================================
// 
//=============================================================================
int CPlayer::PopString(char * pSrc, char * pDst)
{
	int nCount = 0;
	while (1)
	{
		pSrc++;
		nCount++;

		if (memcmp(pSrc, " ", strlen(" ")) == 0)
		{
			pSrc = "\0";
			nCount++;
			break;
		}
		else if (memcmp(pSrc, "\t", strlen("\t")) == 0)
		{
			pSrc = "\0";
			nCount++;
			break;
		}
	}
	strcpy(pDst, pSrc);

	return nCount;
}

//=============================================================================
// �v���C���[�̐���
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			pPlayer->Init(pos);
		}
	}
	return pPlayer;
}
//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// �ړ��擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}
//=============================================================================
// ��Ԏ擾
//=============================================================================
CPlayer::PLAYERSTATE CPlayer::GetState(void)
{
	return m_state;
}
//=============================================================================
// �p�x�̐ݒ�
//=============================================================================
void CPlayer::SetAngle(void)
{
	m_fDiff = m_fDest - m_rot.y;

	if (m_fDiff > D3DX_PI)
	{
		m_fDiff -= D3DX_PI* PLAYER_AROUND;
	}
	if (m_fDiff < -D3DX_PI)
	{
		m_fDiff += D3DX_PI* PLAYER_AROUND;
	}

	m_rot.y += m_fDiff * PLAYER_DIFF;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI* PLAYER_AROUND;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI* PLAYER_AROUND;
	}
}
//=============================================================================
// �}�b�v�����蔻��
//=============================================================================
void CPlayer::MapCollision(void)
{
	if (m_pos.z > -PLAYER_MIN_POS)
	{// Z�̚�
		m_pos.z = -PLAYER_MIN_POS;
	}
	else if (m_pos.z < -PLAYER_MAX_POS)
	{// Z�̎�O
		m_pos.z = -PLAYER_MAX_POS;
	}
	else if (m_pos.x < PLAYER_MIN_POS)
	{// X�̍�
		m_pos.x = PLAYER_MIN_POS;
	}
	else if (m_pos.x > PLAYER_MAX_POS)
	{// X�̉E
		m_pos.x = PLAYER_MAX_POS;
	}
}
//=============================================================================
// �����蔻��S��
//=============================================================================
void CPlayer::CollisionAll(void)
{
	CScene *pScene = NULL;

	pScene = CScene::GetTop(3);

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetNext();	// �⌾  /* �X�V�����Ƃ��ɏ�����\�������邩�� */

		if (pScene->GetDeath() == false)
		{// ���S�t���O�������Ă�����

			CScene::OBJTYPE objType;

			// �I�u�W�F�N�g�̎擾
			objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_OBJECT)
			{// �I�u�W�F�N�g��������
				((CObject*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
			else if (objType == CScene::OBJTYPE_ENEMY)
			{// �G��������
				((CEnemy*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
			else if (objType == CScene::OBJTYPE_GRASS)
			{// ����������
				((CGrass*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
		}
		// ���ɐi�߂�
		pScene = pSceneNext;
	}
}
//=============================================================================
// �����蔻��
//=============================================================================
bool CPlayer::AttackCollision(D3DXVECTOR3 pos, float radius)
{
	m_bHit = false;	// �����������ǂ���

	if (m_bAttack == true)
	{// �v���C���[���U����ԂȂ瓖���蔻��ON

		// ����̃��[���h�}�g���b�N�X�擾
		float fXmtx = pos.x - (m_apModel[10]->GetMtxWorld()._41);
		float fYmtx = pos.y + 30 - (m_apModel[10]->GetMtxWorld()._42);
		float fZmtx = pos.z - (m_apModel[10]->GetMtxWorld()._43);

		// ���v�̌v�Z
		float fTotal = (float)pow(fXmtx, 2) + (float)pow(fYmtx, 2) + (float)pow(fZmtx, 2);	// pow(x,y�jx��y��Ōv�Z�����

		if (fTotal <= radius)
		{// ����ƓG�̔���
			m_bHit = true;
			//CDebugProc::Print(1, "��������\n");
		}
	}
	return m_bHit;
}

