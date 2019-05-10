//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "polygon.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPolygon::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon::CPolygon()
{
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
}
//=============================================================================
// デストラクタ
//=============================================================================
CPolygon::~CPolygon()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - LIFE_SIZE, m_pos.y - LIFE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + LIFE_SIZE, m_pos.y - LIFE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - LIFE_SIZE, m_pos.y + LIFE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + LIFE_SIZE, m_pos.y + LIFE_SIZE, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 色
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CPolygon::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CPolygon::Update(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void CPolygon::Draw(void)
{
	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// ナンバーの設置
//=============================================================================
void CPolygon::SetLife(int nLife)
{
	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f * nLife), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f + (1.0f * nLife), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f * nLife), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + (1.0f * nLife), 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ナンバーの生成
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
// テクスチャの読み込み
//=============================================================================
HRESULT CPolygon::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, LIFE_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPolygon::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
