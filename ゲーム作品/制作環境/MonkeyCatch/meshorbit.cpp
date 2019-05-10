//=============================================================================
//
// ���b�V���I�[�r�b�g���� [meshorbit.cpp]
// Author : �|���j
//
//=============================================================================
#include "meshorbit.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CPlayer *CMeshOrbit::m_pPlayer = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshOrbit::CMeshOrbit():CScene(6)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nNumPos = 0;
	m_nNumPolygon = 0;
	m_fCol = 0.0f;
	m_bUse = false;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshOrbit::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fCol = 0.0f;

	for (int nCount = 0; nCount < MAX_VERTEX; nCount++)
	{// ���_�����J��Ԃ�

		// �@���̐ݒ�
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F
		pVtx[nCount].col = D3DXCOLOR(0.0f, 0.5f, 0.8f, m_fCol -(nCount*0.05f));
	}

	// ���b�V���I�[�r�b�g�̃I�t�Z�b�g
	m_aOffset[0] = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_aOffset[1] = D3DXVECTOR3(0.0f, 40.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CMeshOrbit::Uninit(void)
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

	if (m_pPlayer != NULL)
	{// �v���C���[��NULL����Ȃ�������NULL�ɂ���
		m_pPlayer = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CMeshOrbit::Update(void)
{
	m_fCol += ORBIT_COLOR;
	if (m_fCol >= 1.0f)
	{
		m_fCol = 1.0f;
	}
	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCount = 0; nCount < MAX_VERTEX; nCount++)
	{// ���_�����J��Ԃ�

	 // �@���̐ݒ�
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F
		pVtx[nCount].col = D3DXCOLOR(0.0f, 0.5f, 0.8f, m_fCol - (nCount*0.05f));
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CMeshOrbit::Draw(void)
{
	// �v���C���[�̎擾
	m_pPlayer = CGame::GetPlayer();

	if (m_pPlayer != NULL)
	{// �v���C���[��NULL����Ȃ�������
		if (m_pPlayer->GetState() == m_pPlayer->PLAYERSTATE_MUSIAMI)
		{// �v���C���[�̏�Ԃ����ԏ�Ԃ�������
			D3DXMATRIX	mtxParent;					// �e�̃}�g���b�N�X

			CManager manager;	//�C���X�^���X

			 //���_����ݒ�
			VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

			// �f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice;
			pDevice = manager.GetRenderer()->GetDevice();

			// ���_�o�b�t�@�����b�N
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCount = MAX_VERTEX - 3; nCount >= 0; nCount--)
			{// ����ւ�
				m_aPosVertex[nCount + 2] = m_aPosVertex[nCount];

				pVtx[nCount + 2].pos = m_aPosVertex[nCount + 2];
			}

			// �x�N�g�����g�����X�t�H�[�����ăx�N�g���𓾂�
			D3DXVec3TransformCoord(&m_aPosVertex[0], &m_aOffset[0], m_pMtxParent);

			pVtx[0].pos = m_aPosVertex[0];

			// �x�N�g�����g�����X�t�H�[�����ăx�N�g���𓾂�
			D3DXVec3TransformCoord(&m_aPosVertex[1], &m_aOffset[1], m_pMtxParent);

			pVtx[1].pos = m_aPosVertex[1];

			// ���_�o�b�t�@���A�����b�N
			m_pVtxBuff->Unlock();

			// �J�����OON
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// �|�����킹��
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_mtxWorld);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_POLYGON);

			// �J�����OOFF
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
	}
}
//=============================================================================
// ����
//=============================================================================
CMeshOrbit * CMeshOrbit::Create(D3DXVECTOR3 pos)
{
	CMeshOrbit *pMeshOrbit = NULL;

	if (pMeshOrbit == NULL)
	{
		pMeshOrbit = new CMeshOrbit;

		if (pMeshOrbit != NULL)
		{
			pMeshOrbit->Init(pos);
		}
	}
	return pMeshOrbit;
}
//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// �e�̃}�g���b�N�X�̐ݒu
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtx)
{
	m_pMtxParent = pMtx;
}
//=============================================================================
// ���[���h�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX CMeshOrbit::GetMtxWorld(void)
{
	return m_mtxWorld;
}
