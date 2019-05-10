//=============================================================================
//
// �G���� [enemy.cpp]
// Author : �|���j
//
//=============================================================================
#include "enemy.h"
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"

#include "scene3D.h"
#include "meshfield.h"
#include "meshorbit.h"
#include "object.h"
#include "particle.h"
#include "grass.h"

#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "remain.h"
#include "score.h"
#include "timer.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL;
LPD3DXMESH CEnemy::m_apMesh[MAX_ENEMY_MODEL] = {};
LPD3DXBUFFER CEnemy::m_apBuffMat[MAX_ENEMY_MODEL] = {};
DWORD CEnemy::m_anNumMat[MAX_ENEMY_MODEL] = {};

CMeshOrbit * CEnemy::m_pMeshOrbit = NULL;
CObject * CEnemy::m_pObject = NULL;
CPlayer * CEnemy::m_pPlayer = NULL;
int CEnemy::m_nNumEnemy = 0;
int CEnemy::m_nGetEnemy = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = ENEMYSTATE_NONE;

	m_pKeyInfo = NULL;
	m_nKey = 0;
	m_nCouterMotion = 0;
	m_nNumParts = 0;
	m_StateSave = 0;

	for (int nCnt = 0; nCnt < MAX_ENEMY_MODEL; nCnt++)
	{
		m_apModel[nCnt] = NULL;
		m_aIndexParent[nCnt] = 0;
		m_aPosOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_bUse = false;
	m_bHit = false;
	m_bHitGrass = false;
	m_bHitObject = false;

	m_type = ENEMYTYPE_000;
	m_MoveCounter = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// ���[�V�����̓ǂݍ���
	LoadMotion();

	// �I�u�W�F�N�g�̃^�C�v�̐ݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	// ���̏�����
	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = ENEMYSTATE_NONE;

	m_bJump = false;
	m_bHitGrass = false;
	m_bHitObject = false;

	m_nNumEnemy = NUM_ENEMY;	// �G�̑���
	m_nGetEnemy = 0;			// �Q�b�g�����G�̐�
	m_nKey = 0;
	m_MoveCounter = 0;

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
void CEnemy::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		//m_apModel[nCntModel]->Uninit();

		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{// ���[�h���`���[�g���A��
		UpdateTutorial();
	}

	if (mode == CManager::MODE_GAME)
	{
		// �J�����̎擾
		CCamera *pCamera = CGame::GetCamera();
		D3DXVECTOR3 rot = pCamera->GetCamera();

		// �ߋ��̈ʒu���擾
		m_posold = m_pos;

		m_pos.y -= m_move.y;
		m_move.y += ENEMY_GRAVITY;		// �d�͉��Z

		// ���b�V���t�B�[���h�̎擾
		CMeshField *pMeshField = CGame::GetMeshField();
		float fHight = pMeshField->GetHeight(m_pos);

		if (m_pos.y < fHight)
		{
			m_pos.y = fHight;
		}

		if (m_type == ENEMYTYPE_000)
		{// �B��Ă�����
			m_fDest = (D3DX_PI * 0.0f);
		}
		if (m_type == ENEMYTYPE_001)
		{// �������p
			if (m_bHitGrass == false || m_bHitObject == false)
			{
				m_move.x += ENEMY_MOVE000;

				if (m_move.x <= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * 0.5f);
				}
				if (m_move.x >= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * -0.5f);
				}
			}

			if (m_bHitGrass == true || m_bHitObject == true)
			{// ���ɓ���������
				m_MoveCounter++;

				m_move.z -= ENEMY_MOVE000;

				if (m_move.z >= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * 1.0f);
				}
				if (m_move.z <= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * 0.0f);
				}
				if (m_MoveCounter == ENEMY_COUNTER000)
				{
					m_bHitGrass = false;
				}
			}
		}

		if (m_type == ENEMYTYPE_002)
		{// ��
			m_move.x += ENEMY_MOVE000;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER001)
			{
				m_move.x *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.x >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * -0.5f);
			}
			if (m_move.x <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.5f);
			}
		}
		if (m_type == ENEMYTYPE_003)
		{// �c
			m_move.z -= ENEMY_MOVE001;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER000)
			{
				m_move.z *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.z >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 1.0f);
			}
			if (m_move.z <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.0f);
			}
		}

		if (m_type == ENEMYTYPE_004)
		{// �� ver2
			m_move.x -= ENEMY_MOVE000;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER001)
			{
				m_move.x *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.x >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * -0.5f);
			}
			if (m_move.x <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.5f);
			}
		}
		if (m_type == ENEMYTYPE_005)
		{// �c ver2
			m_move.z += ENEMY_MOVE001;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER000)
			{
				m_move.z *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.z >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 1.0f);
			}
			if (m_move.z <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.0f);
			}
		}

		// �p�x�̐ݒ�
		SetAngle();

		if (m_move.x < ENEMY_MOVESTAND && m_move.x > -ENEMY_MOVESTAND && m_move.z < ENEMY_MOVESTAND && m_move.z > -ENEMY_MOVESTAND)
		{// �j���[�g����
			m_state = ENEMYSTATE_NORMAL;
		}
		if (m_move.x >= ENEMY_MOVESTAND || m_move.x <= -ENEMY_MOVESTAND || m_move.z >= ENEMY_MOVESTAND || m_move.z <= -ENEMY_MOVESTAND)
		{// �ړ�
			m_state = ENEMYSTATE_MOVE;
		}

		// �ʒu�X�V
		m_pos.x += m_move.x;
		m_pos.z += m_move.z;

		// ����
		m_move.x += (0.0f - m_move.x) * 0.02f;
		m_move.z += (0.0f - m_move.z) * 0.02f;

		// ���[�V�����̍X�V
		UpdateMotion();

		// �����蔻��S��
		CollisionAll();

		// �}�b�v�����蔻��
		MapCollision();

		// �c�@�̎擾
		CRemain * pRemain = CGame::GetRemain();

		// �X�R�A�̎擾
		CScore * pScore = CGame::GetScore();

		// �^�C�}�[�̎擾
		CTimer * pTimer = CGame::GetTimer();

		if (m_bUse == true)
		{// �߂܂�����
			Uninit();

			for (int nCntParticle = 0; nCntParticle < E_MAX_PARTICLE; nCntParticle++)
			{// �p�[�e�B�N���̐���
				CParticle::Create(D3DXVECTOR3(E_PARTICLE_CREATEPOS),
					D3DXVECTOR3(sinf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), cosf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), 0.0f),
					E_PARTICLE_LIFE, E_PARTICLE_RADIUS, CParticle::PARTICLETYPE_000);
			}

			// ���������炷
			m_nNumEnemy--;

			// �Q�b�g���������₷
			m_nGetEnemy++;

			// �c�@���炷
			pRemain->AddRemain(-ENEMY_REMAIN);

			// �X�R�A���Z
			pScore->AddScore(ENEMY_SCORE);
		}

		if (m_nNumEnemy == 0)
		{// �G��0�ɂȂ�����

			// �^�C�}�[�X�g�b�v
			pTimer->StopTimer();

			// �t�F�[�h�̎擾
			CFade *pFade = CManager::GetFade();
			pFade->Set(CManager::MODE_RESULT, pFade->FADE_OUT);
		}
	}
}
//=============================================================================
// �X�V����(�`���[�g���A���p)
//=============================================================================
void CEnemy::UpdateTutorial(void)
{
	// �J�����̎擾
	CCamera *pCamera = CTutorial::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetCamera();

	// �ߋ��̈ʒu���擾
	m_posold = m_pos;

	m_pos.y -= m_move.y;
	m_move.y += ENEMY_GRAVITY;		// �d�͉��Z

	// ���b�V���t�B�[���h�̎擾
	CMeshField *pMeshField = CTutorial::GetMeshField();
	float fHight = pMeshField->GetHeight(m_pos);

	if (m_pos.y < fHight)
	{
		m_pos.y = fHight;
	}

	if (m_type == ENEMYTYPE_000)
	{// �B��Ă�����
		m_fDest = (D3DX_PI * 0.0f);
	}

	// �p�x�̐ݒ�
	SetAngle();

	if (m_move.x < ENEMY_MOVESTAND && m_move.x > -ENEMY_MOVESTAND && m_move.z < ENEMY_MOVESTAND && m_move.z > -ENEMY_MOVESTAND)
	{// �j���[�g����
		m_state = ENEMYSTATE_NORMAL;
	}
	if (m_move.x >= ENEMY_MOVESTAND || m_move.x <= -ENEMY_MOVESTAND || m_move.z >= ENEMY_MOVESTAND || m_move.z <= -ENEMY_MOVESTAND)
	{// �ړ�
		m_state = ENEMYSTATE_MOVE;
	}

	// �ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	// ����
	m_move.x += (0.0f - m_move.x) * 0.02f;
	m_move.z += (0.0f - m_move.z) * 0.02f;

	// ���[�V�����̍X�V
	UpdateMotion();

	// �����蔻��S��
	CollisionAll();

	// �}�b�v�����蔻��
	MapCollision();

	// �X�R�A�̎擾
	CScore * pScore = CTutorial::GetScore();

	if (m_bUse == true)
	{// �߂܂�����
		Uninit();

		for (int nCntParticle = 0; nCntParticle < E_MAX_PARTICLE; nCntParticle++)
		{// �p�[�e�B�N���̐���
			CParticle::Create(D3DXVECTOR3(E_PARTICLE_CREATEPOS),
				D3DXVECTOR3(sinf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), cosf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), 0.0f),
				E_PARTICLE_LIFE, E_PARTICLE_RADIUS,CParticle::PARTICLETYPE_000);
		}

		// ���������炷
		m_nNumEnemy--;

		// �Q�b�g���������₷
		m_nGetEnemy++;

		// �X�R�A���Z
		pScore->AddScore(ENEMY_SCORE);
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
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

	vecLight = ENEMY_VECLIGHT;
	pos = ENEMY_POS;
	normal = ENEMY_NORMAL;

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
//=============================================================================
// ���[�V�����e�L�X�g�̓ǂݍ���
//=============================================================================
void CEnemy::LoadMotion(void)
{
	FILE * pFile;
	char *pStrCur;						// ������̐擪�|�C���^
	char aLine[MAX_CHAR];				// ������ǂݍ��ݗp
	char aStr[MAX_CHAR];				// �����񔲂��o���p
	int nWord;
	int nSpace;
	int nIndex;							// �C���f�b�N�X
	D3DXVECTOR3 aPos[MAX_ENEMY_MODEL];	// �ʒu(�p�[�c��)
	D3DXVECTOR3 aRot[MAX_ENEMY_MODEL];	// ����(�p�[�c��)
	int nLoop;							// ���[�v
	int nCntMotion = 0;					// ���[�V����

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �t�@�C���I�[�v��
	pFile = fopen(ENEMY_FILE, "r");

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

				strcpy(&aStr[0], pStrCur);	// �Ώۂ̕����񔲂��o��

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

							aPos[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							aPos[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							aPos[nIndex].z = (float)atof(pStrCur);	// Z

							// 1�s�ǂݍ���
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], ROT, strlen(ROT)) == 0)
						{// ����
							pStrCur += strlen(ROT);		// ���o��

							aRot[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							aRot[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// ���܂Ői�߂�

							aRot[nIndex].z = (float)atof(pStrCur);	// Z

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
							m_apModel[0] = CModel::Create(D3DXVECTOR3(m_pos.x + aPos[nIndex].x, m_pos.y + aPos[nIndex].y, aPos[nIndex].z));
							m_apModel[0]->BindModel(m_apMesh[nIndex], m_apBuffMat[nIndex], m_anNumMat[nIndex]);
							m_apModel[0]->SetParent(NULL);			// ��

							// �I�t�Z�b�g�ɑ��
							m_aPosOffset[0] = m_apModel[0]->GetPos();
						}
						else if (m_aIndexParent[nIndex] > -1)
						{// �q
							m_apModel[nIndex] = CModel::Create(D3DXVECTOR3(aPos[nIndex].x, aPos[nIndex].y, aPos[nIndex].z));
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
void CEnemy::UpdateMotion(void)
{
	m_StateSave = m_state - 1;

	E_KEY *pKey, *pKeyNext;				// ���݂̃L�[,���̃L�[
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
				m_nKey = 1;
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
}
//=============================================================================
// �����ȍs����
//=============================================================================
char *CEnemy::ReadLine(FILE * pFile, char *pDst)
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
char *CEnemy::GetLineTop(char *pSrc)
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
int CEnemy::PopString(char * pSrc, char * pDst)
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
// �p�x�̐ݒ�
//=============================================================================
void CEnemy::SetAngle(void)
{
	m_fDiff = m_fDest - m_rot.y;

	if (m_fDiff > D3DX_PI)
	{
		m_fDiff -= D3DX_PI* ENEMY_AROUND;
	}
	if (m_fDiff < -D3DX_PI)
	{
		m_fDiff += D3DX_PI* ENEMY_AROUND;
	}

	m_rot.y += m_fDiff * ENEMY_DIFF;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI* ENEMY_AROUND;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI* ENEMY_AROUND;
	}
}
//=============================================================================
// �}�b�v�����蔻��
//=============================================================================
void CEnemy::MapCollision(void)
{
	if (m_pos.z > -ENEMY_MIN_POS)
	{// Z�̚�
		m_pos.z = -ENEMY_MIN_POS;
	}
	else if (m_pos.z < -ENEMY_MAX_POS)
	{// Z�̎�O
		m_pos.z = -ENEMY_MAX_POS;
	}
	else if (m_pos.x < ENEMY_MIN_POS)
	{// X�̍�
		m_pos.x = ENEMY_MIN_POS;
	}
	else if (m_pos.x > ENEMY_MAX_POS)
	{// X�̉E
		m_pos.x = ENEMY_MAX_POS;
	}
}
//=============================================================================
// �����蔻��S��
//=============================================================================
void CEnemy::CollisionAll(void)
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
				bool bObjectHit = ((CObject*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);

				if (bObjectHit == true)
				{// �I�u�W�F�N�g�ɓ���������
					m_bHitObject = bObjectHit;
				}
			}
			if (objType == CScene::OBJTYPE_PLAYER)
			{// �v���C���[�̕���̔���
				bool bHit = ((CPlayer*)pScene)->AttackCollision(m_pos, ENEMY_COLLISION);

				if (bHit == true)
				{// �����蔻��ON
					m_bUse = true;
				}
			}
			if (objType == CScene::OBJTYPE_GRASS)
			{// ����������
				bool bGrassHit = ((CGrass*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);

				if (bGrassHit == true)
				{// ���ɓ���������
					m_bHitGrass = bGrassHit;
				}
			}
		}
		// ���ɐi�߂�
		pScene = pSceneNext;
	}
}
//=============================================================================
// �����蔻��
//=============================================================================
bool CEnemy::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin)
{
	m_bHit = false;		// �����������ǂ���

	if (pPos->y + pVtxMax->y > m_pos.y + m_VtxMin.y && pPos->y + pVtxMin->y < m_pos.y + m_VtxMax.y)
	{// ���f���̍�����
		if (pPos->x + pVtxMax->x > m_pos.x + m_VtxMin.x && pPos->x + pVtxMin->x < m_pos.x + m_VtxMax.x)
		{// Z�ʂ̔���
			if (pPosold->z + pVtxMin->z >= m_pos.z + m_VtxMax.z && m_pos.z + m_VtxMax.z >= pPos->z + pVtxMin->z)
			{// ���̔���
				pPos->z = pPosold->z;
				m_bHit = true;
			}
			if (pPosold->z + pVtxMax->z <= m_pos.z + m_VtxMin.z && m_pos.z + m_VtxMin.z <= pPos->z + pVtxMax->z)
			{// ��O�̔���
				pPos->z = pPosold->z;
				m_bHit = true;
			}
		}

		if (pPos->z + pVtxMax->z > m_pos.z + m_VtxMin.z && pPos->z + pVtxMin->z < m_pos.z + m_VtxMax.z)
		{// X�ʂ̔���
			if (pPosold->x + pVtxMin->x >= m_pos.x + m_VtxMax.x && m_pos.x + m_VtxMax.x >= pPos->x + pVtxMin->x)
			{// �E�ʂ̔���		
				pPos->x = pPosold->x;
				m_bHit = true;
			}
			if (pPosold->x + pVtxMax->x <= m_pos.x + m_VtxMin.x && m_pos.x + m_VtxMin.x <= pPos->x + pVtxMax->x)
			{// ���ʂ̔���
				pPos->x = pPosold->x;
				m_bHit = true;
			}
		}
	}
	return m_bHit;
}
//=============================================================================
// �G�̐���
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			pEnemy->Init(pos);
			pEnemy->SetType(type);	// ���
		}
	}
	return pEnemy;
}
////=============================================================================
// �G�̑����擾
//=============================================================================
int CEnemy::GetNumEnemy(void)
{
	return m_nNumEnemy;
}
//=============================================================================
// �G�̑����擾
//=============================================================================
int CEnemy::GetCatchEnemy(void)
{
	return m_nGetEnemy;
}
//=============================================================================
// �G�̎�ސݒ�
//=============================================================================
void CEnemy::SetType(ENEMYTYPE type)
{
	m_type = type;
}
