//=============================================================================
//
// �|���S������ [polygon.cpp]
// Author : �|���j
//
//=============================================================================
#include "polygon.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPolygon::CPolygon()
{
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPolygon::~CPolygon()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - LIFE_SIZE, m_pos.y - LIFE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + LIFE_SIZE, m_pos.y - LIFE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - LIFE_SIZE, m_pos.y + LIFE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + LIFE_SIZE, m_pos.y + LIFE_SIZE, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
void CPolygon::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CPolygon::Update(void)
{

}
//=============================================================================
// �`�揈��
//=============================================================================
void CPolygon::Draw(void)
{
	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// �i���o�[�̐ݒu
//=============================================================================
void CPolygon::SetLife(int nLife)
{
	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f * nLife), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f + (1.0f * nLife), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f * nLife), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + (1.0f * nLife), 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �i���o�[�̐���
//=============================================================================
CPolygon * CPolygon::Create(D3DXVECTOR3 pos)
{
	CPolygon *pPolygon = NULL;

	if (pPolygon == NULL)
	{
		pPolygon = new CPolygon;

		if (pPolygon != NULL)
		{
			pPolygon->Init(pos);
		}
	}
	return pPolygon;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CPolygon::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, LIFE_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPolygon::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
