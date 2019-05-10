//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : �|���j
//
//=============================================================================
#include "light.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void CLight::Init(void)
{
	CManager manager;	//�C���X�^���X

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�̎擾
	pDevice = manager.GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;

	// ���C�g���N���A����
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));
	}

	// ���C�g�̎�ނ�ݒ�
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ�
	m_aLight[0].Diffuse = LIGHT_DIFFUSE000;
	m_aLight[1].Diffuse = LIGHT_DIFFUSE001;
	m_aLight[2].Diffuse = LIGHT_DIFFUSE002;

	// ���C�g�̕����̐ݒ�
	vecDir = LIGHT_VECDIR000;
	D3DXVec3Normalize(&vecDir, &vecDir);			// ���K������
	m_aLight[0].Direction = vecDir;

	vecDir = LIGHT_VECDIR001;
	D3DXVec3Normalize(&vecDir, &vecDir);			// ���K������
	m_aLight[1].Direction = vecDir;

	vecDir = LIGHT_VECDIR002;
	D3DXVec3Normalize(&vecDir, &vecDir);			// ���K������
	m_aLight[2].Direction = vecDir;

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &m_aLight[0]);
	pDevice->SetLight(1, &m_aLight[1]);
	pDevice->SetLight(2, &m_aLight[2]);

	// ���C�g��L���ɂ���
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		pDevice->LightEnable(nCntLight, TRUE);
	}
}
//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void)
{

}
//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void)
{

}

