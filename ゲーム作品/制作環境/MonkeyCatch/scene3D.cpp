//=============================================================================
//
// �I�u�W�F�N�g3D���� [scene3D.cpp]
// Author : �|���j
//
//=============================================================================
#include "scene3D.h"
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
CScene3D::CScene3D()
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0,vec1;
	D3DXVECTOR3 nor0,nor1;

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	 //�ʒu
	pVtx[0].pos = D3DXVECTOR3(-SCENE3D_SIZE, 0.0f, -SCENE3D_SIZE);
	pVtx[1].pos = D3DXVECTOR3(-SCENE3D_SIZE, 0.0f ,SCENE3D_SIZE);
	pVtx[2].pos = D3DXVECTOR3(SCENE3D_SIZE, 0.0f, -SCENE3D_SIZE);
	pVtx[3].pos = D3DXVECTOR3(SCENE3D_SIZE, 0.0f, SCENE3D_SIZE);
	
	vec0 = pVtx[3].pos - pVtx[2].pos;
	vec1 = pVtx[3].pos - pVtx[1].pos;

	D3DXVec3Cross(&nor0, &vec0, &vec1);
	D3DXVec3Normalize(&nor0, &nor0);	// ���K������

	pVtx[3].nor = nor1;

	vec0 = pVtx[0].pos - pVtx[1].pos;
	vec1 = pVtx[0].pos - pVtx[2].pos;

	D3DXVec3Cross(&nor1, &vec0, &vec1);
	D3DXVec3Normalize(&nor1, &nor1);	// ���K������

	pVtx[0].nor = nor0;

	// �@���̐ݒ�
	pVtx[1].nor = (nor0 + nor1) / 2;;
	pVtx[2].nor = (nor0 + nor1) / 2;;
	
	// �F
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CScene3D::Update(void)
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor0, nor1;

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	vec0 = pVtx[3].pos - pVtx[2].pos;
	vec1 = pVtx[3].pos - pVtx[1].pos;

	D3DXVec3Cross(&nor0, &vec0, &vec1);
	D3DXVec3Normalize(&nor0, &nor0);	// ���K������

	pVtx[3].nor = nor0;

	vec0 = pVtx[0].pos - pVtx[1].pos;
	vec1 = pVtx[0].pos - pVtx[2].pos;

	D3DXVec3Cross(&nor1, &vec0, &vec1);
	D3DXVec3Normalize(&nor1, &nor1);	// ���K������

	pVtx[0].nor = nor1;

	// �@���̐ݒ�
	pVtx[1].nor = (nor0 + nor1) / 2;;
	pVtx[2].nor = (nor0 + nor1) / 2;;

	CDebugProc::Print(1, "pVtx[0] %.1f,%.1f,%.1f\n", pVtx[0].nor.x, pVtx[0].nor.y, pVtx[0].nor.z);
	CDebugProc::Print(1, "pVtx[1] %.1f,%.1f,%.1f\n", pVtx[1].nor.x, pVtx[1].nor.y, pVtx[1].nor.z);
	CDebugProc::Print(1, "pVtx[2] %.1f,%.1f,%.1f\n", pVtx[2].nor.x, pVtx[2].nor.y, pVtx[2].nor.z);
	CDebugProc::Print(1, "pVtx[3] %.1f,%.1f,%.1f\n\n", pVtx[3].nor.x, pVtx[3].nor.y, pVtx[3].nor.z);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// ����
//=============================================================================
CScene3D * CScene3D::Create(D3DXVECTOR3 pos)
{
	CScene3D *pScene3D = NULL;

	if (pScene3D == NULL)
	{
		pScene3D = new CScene3D;

		if (pScene3D != NULL)
		{
			pScene3D->Init(pos);
		}
	}
	return pScene3D;
}
//=============================================================================
// �ʒu�̐ݒu
//=============================================================================
void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu
	pVtx[0].pos = D3DXVECTOR3(-SCENE3D_SIZE, 0.0f,  +SCENE3D_SIZE);
	pVtx[1].pos = D3DXVECTOR3(+SCENE3D_SIZE, 0.0f,  +SCENE3D_SIZE);
	pVtx[2].pos = D3DXVECTOR3( -SCENE3D_SIZE, 0.0f,  -SCENE3D_SIZE);
	pVtx[3].pos = D3DXVECTOR3( +SCENE3D_SIZE, 0.0f,  -SCENE3D_SIZE);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CScene3D::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// ���_�o�b�t�@�̎擾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene3D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// �����擾
//=============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	float fGaiseki;	// �O��

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_vecA = pVtx[1].pos - pVtx[2].pos;
	m_vecC = pos - pVtx[2].pos;

	fGaiseki = (m_vecA.z *  m_vecC.x) - (m_vecA.x * m_vecC.z);

	if (fGaiseki >= 0)
	{
		pos.y = ((-pVtx[3].nor.x * (pos.x - pVtx[3].pos.x)) + (-pVtx[3].nor.z * (pos.z - pVtx[3].pos.z)) / pVtx[3].nor.y) + pVtx[3].pos.y;
	}

	m_vecA = pVtx[2].pos - pVtx[1].pos;
	m_vecC = pos - pVtx[1].pos;

	fGaiseki = (m_vecA.z *  m_vecC.x) - (m_vecA.x * m_vecC.z);

	if (fGaiseki > 0)
	{
		pos.y = ((-pVtx[0].nor.x * (pos.x - pVtx[0].pos.x)) + (-pVtx[0].nor.z * (pos.z - pVtx[0].pos.z)) / pVtx[0].nor.y) + pVtx[0].pos.y;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return pos.y;
}



