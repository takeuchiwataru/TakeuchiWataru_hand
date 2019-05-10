//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.cpp]
// Author : �|���j
//
//=============================================================================
#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_apTexture[PARTICLETYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle()
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius)
{
	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_PARTICLE);

	CBillBoard::Init(pos);

	if (m_type == PARTICLETYPE_000)
	{//
		CBillBoard::SetPosition(pos, PARTICLE_SIZE000, PARTICLE_SIZE000);
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	}
	if (m_type == PARTICLETYPE_001)
	{//
		CBillBoard::SetPosition(pos, PARTICLE_SIZE000, PARTICLE_SIZE000);
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ���̏�����
	m_move = move;
	m_nLife = life;
	m_nRadius = radius;

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	CBillBoard::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	if (m_type == PARTICLETYPE_000)
	{// 
		m_nLife--;

		m_nRadius -= m_nRadius / m_nLife;

		// �ʒu�̎擾
		D3DXVECTOR3 pos = CBillBoard::GetPosition();

		pos -= m_move;

		CBillBoard::SetPosition(pos, PARTICLE_SIZE000, PARTICLE_SIZE000);
		CBillBoard::SetColor(m_col);
		CBillBoard::SetParticle(m_nRadius, pos);

		if (m_nLife <= 0 || m_nRadius == PARTICLE_SIZE000)
		{
			Uninit();
		}
	}
	if (m_type == PARTICLETYPE_001)
	{// 
		m_nLife--;

		m_nRadius -= m_nRadius / m_nLife;

		// �ʒu�̎擾
		D3DXVECTOR3 pos = CBillBoard::GetPosition();

		pos -= m_move;
		m_move.y -= PARTICLE_GRAVITY;		// �d�͉��Z

		CBillBoard::SetPosition(pos, PARTICLE_SIZE000, PARTICLE_SIZE000);
		CBillBoard::SetColor(m_col);
		CBillBoard::SetParticle2(m_nRadius, pos);

		if (m_nLife <= 0 || m_nRadius == PARTICLE_SIZE001)
		{
			Uninit();
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillBoard::Draw();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
// �p�[�e�B�N���̎�ނ̐ݒ�
//=============================================================================
void CParticle::SetType(PARTICLETYPE type)
{
	m_type = type;
}
//=============================================================================
// �p�[�e�B�N���̐���
//=============================================================================
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius, PARTICLETYPE type)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init(pos, move, life, radius);
			pParticle->SetType(type);
			pParticle->BindTexture(m_apTexture[type]);
		}
	}
	return pParticle;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CParticle::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_000, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_001, &m_apTexture[1]);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CParticle::Unload(void)
{
	for (int nCnt = 0; nCnt < PARTICLETYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}