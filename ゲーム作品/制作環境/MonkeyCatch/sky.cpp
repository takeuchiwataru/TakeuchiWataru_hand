//=============================================================================
//
// �󏈗� [sky.cpp]
// Author : �|���j
//
//=============================================================================
#include "sky.h"
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
CSky::CSky()
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_apVtxBuff[0] = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	m_apVtxBuff[1] = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Length = 0;
	m_Side = 0;
	m_XBlock = 0;
	m_YBlock = 0;
	m_Addtex = 0.0f;		
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSky::~CSky()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CSky::Init(D3DXVECTOR3 pos)
{	
	// �e�������̐ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Length = DOME_LENGTH;
	m_Side = DOME_SIDE;
	m_XBlock = BLOCK_SIDE;
	m_YBlock = BLOCK_LENGTH;

	m_nNumVertex = (m_XBlock + 1) * (m_YBlock + 1);		// ���_��
	m_nNumIndex = ((m_XBlock + 1) * (m_YBlock + 1)) + (2 * (m_YBlock - 1)) + (m_XBlock + 1) * (m_YBlock - 1);		// �C���f�b�N�X��
	m_nNumPolygon = ((m_XBlock * m_YBlock) * 2) + ((m_YBlock - 1) * 4);	// �|���S����

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,SKY_TEXTURE_NAME,&m_pTexture);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_apVtxBuff[0],
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_apVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // �p�x
	float fRadius = 0;  // ���a

	fAngle = 0.0f;							// �p�x��0��
	fRadius = cosf(0.0f) * DOME_RADIUS;		// ���a��ݒ�

	for (int nCntV = 0; nCntV < m_YBlock + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < m_XBlock + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(-fAngle * D3DX_PI) * fRadius;
			pos.y = sinf(0.15f * nCntV) * DOME_HEIGHT - 500.0f;
			pos.z = cosf(-fAngle * D3DX_PI) * fRadius;

			pVtx[nCntH].pos = pos;
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntH].tex = D3DXVECTOR2(0.0f + 1.0f / (m_XBlock / 2), 0.0f + (1.0f * nCntV));

			fAngle += 1.0f / (m_XBlock / 2);	// �p�x��i�߂�

			if (fAngle >= 1.0f)
			{// �{����1�𒴂���
				fAngle = -1.0f;
			}
		}
		fRadius = cosf(0.26f * (nCntV + 1)) * DOME_RADIUS;	// ���a�����X�Ɍ���������
		fAngle = 0.0f;										// �p�x��0��
		pVtx += m_XBlock + 1;								// ���_�̐擪�̔ԍ���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	m_apVtxBuff[0]->Unlock();


	WORD *pIdx;      // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0; // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_YBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_XBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		 // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_XBlock + 1; // ����
			pIdx[1] = nCntIdx;                    // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_YBlock && nCntIdxX == m_XBlock)
			{// 1 , �������c�̕������̍ŉ��w�ł͂Ȃ�
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;						// �㑤
				pIdx[1] = nCntIdx + (m_XBlock + 1) + 1;	// ���̉���

				pIdx += 2;	// 2���i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CSky::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apVtxBuff[nCnt] != NULL)
		{
			m_apVtxBuff[nCnt]->Release();
			m_apVtxBuff[nCnt] = NULL;
		}
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
void CSky::Update(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_apVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);

	m_Addtex += SKY_ROT;  // �����镪��i�߂�

	for (int nCntV = 0; nCntV <m_YBlock + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH <m_XBlock + 1; nCntH++)
		{//���������̕����������J��Ԃ�
		 // �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(0.0f + (nCntH * (1.0f /m_XBlock)) + m_Addtex, 0.0f - (nCntV * (1.0f /m_YBlock)));
		}
		pVtx +=m_XBlock + 1;       // ���_�̐擪�̔ԍ���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	m_apVtxBuff[0]->Unlock();

	m_rot.y += SKY_ROT;
}
//=============================================================================
// �`�揈��
//=============================================================================
void CSky::Draw(void)
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
	pDevice->SetStreamSource(0, m_apVtxBuff[0], 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CSky::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// ����
//=============================================================================
CSky * CSky::Create(D3DXVECTOR3 pos)
{
	CSky *pSky = NULL;

	if (pSky == NULL)
	{
		pSky = new CSky;

		if (pSky != NULL)
		{
			pSky->Init(pos);
		}
	}
	return pSky;
}
