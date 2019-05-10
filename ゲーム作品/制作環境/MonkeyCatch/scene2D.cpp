//=============================================================================
//
// オブジェクト2D処理 [scene2D.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D()
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, SCENE2D_TEXTURE, &m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置
	pVtx[0].pos = D3DXVECTOR3(m_pos.x -SCENE2D_SIZE, m_pos.y -SCENE2D_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x +SCENE2D_SIZE, m_pos.y -SCENE2D_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x -SCENE2D_SIZE, m_pos.y +SCENE2D_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x +SCENE2D_SIZE, m_pos.y +SCENE2D_SIZE, 0.0f);

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
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetPress(DIK_SPACE) == true)
	//{// 確認用
	//	m_pNext->Uninit();

	//	Uninit();
	//}
}
//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
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
// 生成
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
// 位置の設置
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y - fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y - fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y + fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y + fheight, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 頂点バッファの取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// カラーの設置
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// パーティクルの設置
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

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fRadius, m_pos.y - fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fRadius, m_pos.y - fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fRadius, m_pos.y + fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fRadius, m_pos.y + fRadius, 0.0f);

	pVtx[0].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[1].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[2].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[3].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

