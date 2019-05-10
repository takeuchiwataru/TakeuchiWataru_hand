//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "light.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void CLight::Init(void)
{
	CManager manager;	//インスタンス

	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = manager.GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;

	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));
	}

	// ライトの種類を設定
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	m_aLight[0].Diffuse = LIGHT_DIFFUSE000;
	m_aLight[1].Diffuse = LIGHT_DIFFUSE001;
	m_aLight[2].Diffuse = LIGHT_DIFFUSE002;

	// ライトの方向の設定
	vecDir = LIGHT_VECDIR000;
	D3DXVec3Normalize(&vecDir, &vecDir);			// 正規化する
	m_aLight[0].Direction = vecDir;

	vecDir = LIGHT_VECDIR001;
	D3DXVec3Normalize(&vecDir, &vecDir);			// 正規化する
	m_aLight[1].Direction = vecDir;

	vecDir = LIGHT_VECDIR002;
	D3DXVec3Normalize(&vecDir, &vecDir);			// 正規化する
	m_aLight[2].Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(0, &m_aLight[0]);
	pDevice->SetLight(1, &m_aLight[1]);
	pDevice->SetLight(2, &m_aLight[2]);

	// ライトを有効にする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		pDevice->LightEnable(nCntLight, TRUE);
	}
}
//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{

}
//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{

}

