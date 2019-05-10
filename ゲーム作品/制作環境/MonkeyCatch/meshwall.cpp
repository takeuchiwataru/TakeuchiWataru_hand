//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.cpp]
// Author : �|���j
//
//=============================================================================
#include "meshwall.h"
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
CMeshWall::CMeshWall()
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_nVtxIndex = 0;
	m_nIdxIndex = 0;
	m_fPos_X = 0;
	m_fPos_Y = 0;
	m_fXpos = 0;
	m_fYpos = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshWall::~CMeshWall()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshWall::Init(D3DXVECTOR3 pos)
{
	int nCntVtx = 0;
	int nCntIdx = 0;

	// �e�������̐ݒ�
	m_nNumVertex = VERTEX_MW_KAZU;		// ���_��
	m_nNumIndex = INDEX_MW_KAZU;		// �C���f�b�N�X��

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, MW_TEXTURE, &m_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_MW_KAZU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_MW_KAZU,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 2�o�C�g�̃f�[�^
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_MW_Y + 1; nCntY++)
	{// �c�̖ʂ̐������J��Ԃ�
		for (int nCntX = 0; nCntX < MAX_MW_X + 1; nCntX++)
		{// ���̖ʂ̐������J��Ԃ�
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].pos = D3DXVECTOR3(m_fPos_X + (nCntX * X_POSSIZE), m_fPos_Y - (nCntY * Y_POSSIZE), 0.0f);
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].tex = D3DXVECTOR2(0 + (nCntX * 0.5f), 0 + (nCntY * 0.5f));
		}
		// ���_���̒l��ێ�
		nCntVtx += MAX_MW_X;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();


	WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < MAX_MW_Y; nCntIdxY++)
	{
		for (int nCntIdxX = 0; nCntIdxX < MAX_MW_X + 1; nCntIdxX++, nCntIdx++)
		{
			pIdx[0] = nCntIdx + MAX_MW_X + 1;
			pIdx[1] = nCntIdx;

			pIdx += 2;			// pIdx��2�����炷

			if (nCntIdxY < MAX_MW_Y && nCntIdxX == MAX_MW_X)
			{
				pIdx[0] = nCntIdx;
				pIdx[1] = nCntIdx + (MAX_MW_X + 1) + 1;

				pIdx += 2;				//pIdx��2�����炷
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CMeshWall::Uninit(void)
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

	// �C���f�b�N�X�o�b�t�@�̉��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CMeshWall::Update(void)
{

}
//=============================================================================
// �`�揈��
//=============================================================================
void CMeshWall::Draw(void)
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

	// �C���f�b�N�X�f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_MW_KAZU, 0, INDEX_MW_KAZU - 2);
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CMeshWall::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// ����
//=============================================================================
CMeshWall * CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CMeshWall *pMeshWall = NULL;

	if (pMeshWall == NULL)
	{
		pMeshWall = new CMeshWall;

		if (pMeshWall != NULL)
		{
			pMeshWall->Init(pos);
			pMeshWall->m_rot = rot;
		}
	}
	return pMeshWall;
}