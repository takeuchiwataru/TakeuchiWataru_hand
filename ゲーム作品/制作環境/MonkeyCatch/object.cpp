//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : �|���j
//
//=============================================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

#include "shadow.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 *CObject::m_pTexture = NULL;
LPD3DXMESH CObject::m_pMesh = NULL;
LPD3DXBUFFER CObject::m_pBuffMat = NULL;
DWORD CObject::m_nNumMat = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject::CObject()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type)
{
	CSceneX::BindModelTex(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat);
	CSceneX::Init(pos);

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �I�u�W�F�N�g�̃^�C�v�̐ݒ�
	SetObjType(CScene::OBJTYPE_OBJECT);

	// ���f��������
	m_pos = pos;
	m_rot = rot;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = type;

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CObject::Uninit(void)
{
	if (m_pTexture != NULL)
	{// (��)
		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{// (�Ƌ�)
			if (m_pTexture[nCnt] != NULL)
			{// �e�N�X�`���j��
				m_pTexture[nCnt]->Release();
				m_pTexture[nCnt] = NULL;
			}
		}
	}

	// �������̊J��(���)
	delete[] m_pTexture;
	// NULL������(�X�n)
	m_pTexture = NULL;

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{	

}
//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	CSceneX::Draw();
}
//=============================================================================
// ����
//=============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type)
{
	CObject *pObject = NULL;

	if (pObject == NULL)
	{
		pObject = new CObject;

		pObject->Init(pos, rot, type);
		CShadow::Create(D3DXVECTOR3(pos.x, pos.y + OBJ_SHADOWPOS, pos.z));
	}
	return pObject;
}
//=============================================================================
// ���f���̓ǂݍ���
//=============================================================================
HRESULT CObject::LoadX(void)
{
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(OBJECT_MODEL_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		m_pTexture[nCntMat] = NULL;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`���̐���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
		}
	}

	return S_OK;
}
//=============================================================================
// ���f���̔j��
//=============================================================================
void CObject::UnloadX(void)
{
	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}
//=============================================================================
// �����蔻��
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin)
{
	D3DXVECTOR3 VtxMax = CSceneX::GetMax();
	D3DXVECTOR3 VtxMin = CSceneX::GetMin();

	bool bHit = false;		// �����������ǂ���

	if (pPos->y + pVtxMax->y > m_pos.y + VtxMin.y && pPos->y + pVtxMin->y < m_pos.y + VtxMax.y)
	{// ���f���̍�����
		if (pPos->x + pVtxMax->x > m_pos.x + VtxMin.x && pPos->x + pVtxMin->x < m_pos.x + VtxMax.x)
		{// Z�ʂ̔���
			if (pPosold->z + pVtxMin->z >= m_pos.z + VtxMax.z && m_pos.z + VtxMax.z >= pPos->z + pVtxMin->z)
			{// ���̔���
				pPos->z = pPosold->z;
				bHit = true;
			}
			if (pPosold->z + pVtxMax->z <= m_pos.z + VtxMin.z && m_pos.z + VtxMin.z <= pPos->z + pVtxMax->z)
			{// ��O�̔���
				pPos->z = pPosold->z;
				bHit = true;
			}
		}

		if (pPos->z + pVtxMax->z > m_pos.z + VtxMin.z && pPos->z + pVtxMin->z < m_pos.z + VtxMax.z)
		{// X�ʂ̔���
			if (pPosold->x + pVtxMin->x >= m_pos.x + VtxMax.x && m_pos.x + VtxMax.x >= pPos->x + pVtxMin->x)
			{// �E�ʂ̔���		
				pPos->x = pPosold->x;
				bHit = true;
			}
			if (pPosold->x + pVtxMax->x <= m_pos.x + VtxMin.x && m_pos.x + VtxMin.x <= pPos->x + pVtxMax->x)
			{// ���ʂ̔���
				pPos->x = pPosold->x;
				bHit = true;
			}
		}
	}
	if (pPos->z > m_pos.z + VtxMin.z + pVtxMin->z && pPos->z < m_pos.z + VtxMax.z + pVtxMax->z &&
		pPos->x > m_pos.x + VtxMin.x + pVtxMin->x && pPos->x < m_pos.x + VtxMax.x + pVtxMax->x)
	{// ���f���̉�
		if (pPosold->y >= m_pos.y + VtxMax.y && pPos->y <= m_pos.y + VtxMax.y)
		{// ��̔���
			pPos->y = pPosold->y;
			bHit = true;
		}
	}
	return bHit;
}
