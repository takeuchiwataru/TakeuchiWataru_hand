//=============================================================================
//
// パーティクルの処理 [particle.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_apTexture[PARTICLETYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius)
{
	// オブジェクトの種類の設定
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

	// 情報の初期化
	m_move = move;
	m_nLife = life;
	m_nRadius = radius;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	CBillBoard::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	if (m_type == PARTICLETYPE_000)
	{// 
		m_nLife--;

		m_nRadius -= m_nRadius / m_nLife;

		// 位置の取得
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

		// 位置の取得
		D3DXVECTOR3 pos = CBillBoard::GetPosition();

		pos -= m_move;
		m_move.y -= PARTICLE_GRAVITY;		// 重力加算

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
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillBoard::Draw();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
// パーティクルの種類の設定
//=============================================================================
void CParticle::SetType(PARTICLETYPE type)
{
	m_type = type;
}
//=============================================================================
// パーティクルの生成
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
// テクスチャの読み込み
//=============================================================================
HRESULT CParticle::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_000, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_001, &m_apTexture[1]);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CParticle::Unload(void)
{
	for (int nCnt = 0; nCnt < PARTICLETYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}