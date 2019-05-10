//=============================================================================
//
// �I�u�W�F�N�g2D���� [scene2D.cpp]
// Author : �|���j
//
//=============================================================================
#include "scene2D.h"
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
CScene2D::CScene2D()
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, SCENE2D_TEXTURE, &m_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu
	pVtx[0].pos = D3DXVECTOR3(m_pos.x -SCENE2D_SIZE, m_pos.y -SCENE2D_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x +SCENE2D_SIZE, m_pos.y -SCENE2D_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x -SCENE2D_SIZE, m_pos.y +SCENE2D_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x +SCENE2D_SIZE, m_pos.y +SCENE2D_SIZE, 0.0f);

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
void CScene2D::Uninit(void)
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
void CScene2D::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetPress(DIK_SPACE) == true)
	//{// �m�F�p
	//	m_pNext->Uninit();

	//	Uninit();
	//}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
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
// ����
//=============================================================================
CScene2D * CScene2D::Create(D3DXVECTOR3 pos)
{
	CScene2D *pScene2D = NULL;

	if (pScene2D == NULL)
	{
		pScene2D = new CScene2D;

		if (pScene2D != NULL)
		{
			pScene2D->Init(pos);
		}
	}
	return pScene2D;
}
//=============================================================================
// �ʒu�̐ݒu
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y - fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y - fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y + fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y + fheight, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// ���_�o�b�t�@�̎擾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// �J���[�̐ݒu
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �p�[�e�B�N���̐ݒu
//=============================================================================
void CScene2D::SetParticle(int nRadius, D3DXVECTOR3 pos)
{
	D3DXCOLOR Col;

	if (nRadius < 1) nRadius = 1;

	float fRadius = (rand() % nRadius + 1) * 1.0f;
	int nNumber = rand() % 3 + 1;

	if (nNumber == 1) { Col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); }
	if (nNumber == 2) { Col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f); }
	if (nNumber == 3) { Col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f); }

	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fRadius, m_pos.y - fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fRadius, m_pos.y - fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fRadius, m_pos.y + fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fRadius, m_pos.y + fRadius, 0.0f);

	pVtx[0].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[1].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[2].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[3].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

