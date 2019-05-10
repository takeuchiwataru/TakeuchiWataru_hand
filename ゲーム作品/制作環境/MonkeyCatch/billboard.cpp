//=============================================================================
//
// ビルボード処理 [billboard.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "billboard.h"
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
CBillBoard::CBillBoard():CScene(6)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CBillBoard::~CBillBoard()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillBoard::Init(D3DXVECTOR3 pos)
{
	// オブジェクトのタイプの設定
	SetObjType(CScene::OBJTYPE_BILLBOARD);

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE,BILLBOARD_SIZE,0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE,BILLBOARD_SIZE,0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE,-BILLBOARD_SIZE,0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE, -BILLBOARD_SIZE,0.0f);

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
void CBillBoard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBillBoard::Update(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void CBillBoard::Draw(void)
{
	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	D3DXMATRIX mtxView, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}
//=============================================================================
// 生成
//=============================================================================
CBillBoard * CBillBoard::Create(D3DXVECTOR3 pos)
{
	CBillBoard *pBillBoard = NULL;

	if (pBillBoard == NULL)
	{
		pBillBoard = new CBillBoard;

		if (pBillBoard != NULL)
		{
			pBillBoard->Init(pos);
		}
	}
	return pBillBoard;
}
//=============================================================================
// 位置の設置
//=============================================================================
void CBillBoard::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	m_pos = pos;

	m_move = move;
}
//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CBillBoard::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CBillBoard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 頂点バッファの取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CBillBoard::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// 位置の設置
//=============================================================================
void CBillBoard::SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-fwidth, fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fwidth, fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fwidth, -fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fwidth, -fheight, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// カラーの設置
//=============================================================================
void CBillBoard::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

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
void CBillBoard::SetParticle(int nRadius, D3DXVECTOR3 pos)
{
	D3DXCOLOR Col;

	if (nRadius < 1) nRadius = 1;

	float fRadius = (rand() % nRadius + 1) * 1.0f;
	int nNumber = rand() % 3 + 1;

	if (nNumber == 1) { Col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); }
	if (nNumber == 2) { Col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f); }
	if (nNumber == 3) { Col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f); }

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRadius,fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRadius,-fRadius, 0.0f);

	pVtx[0].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[1].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[2].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[3].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// パーティクルの設置
//=============================================================================
void CBillBoard::SetParticle2(int nRadius, D3DXVECTOR3 pos)
{
	D3DXCOLOR Col;

	if (nRadius < 1) nRadius = 1;

	float fRadius = (rand() % nRadius + 1) * 1.0f;
	int nNumber = rand() % 3 + 1;

	if (nNumber == 1) { Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
	if (nNumber == 2) { Col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f); }
	if (nNumber == 3) { Col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f); }

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRadius, -fRadius, 0.0f);

	pVtx[0].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[1].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[2].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);
	pVtx[3].col = D3DXCOLOR(Col.r, Col.g, Col.b, Col.a);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
