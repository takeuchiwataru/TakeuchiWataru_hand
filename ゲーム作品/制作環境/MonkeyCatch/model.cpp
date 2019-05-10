//=============================================================================
//
// ���f������ [model.cpp]
// Author : �|���j
//
//=============================================================================
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pParent = NULL;		// �e���f���̃|�C���^
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���f��������
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);
	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
}
//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{

}
//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DXMATRIX	mtxParent;						// �e�̃}�g���b�N�X

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

	// �e�̃}�g���b�N�X�𔽉f
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`��
		pDevice->SetTexture(0, m_pTexture);

		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}
//=============================================================================
// ����
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{
		pModel = new CModel;

		pModel->Init(pos);
	}
	return pModel;
}
//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// ���f���̊��蓖��
//=============================================================================
void CModel::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = pNumMat;
}
//=============================================================================
// ���_�o�b�t�@�̎擾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CModel::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// �e�̐ݒu
//=============================================================================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}
//=============================================================================
// ��]�̐ݒu
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}
//=============================================================================
// �ʒu�̐ݒu
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//=============================================================================
// ���[���h�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX &CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}


