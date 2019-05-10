//=============================================================================
//
// オブジェクト3D処理 [scene3D.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "scene3D.h"
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
CScene3D::CScene3D()
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0,vec1;
	D3DXVECTOR3 nor0,nor1;

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture);

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

	 //位置
	pVtx[0].pos = D3DXVECTOR3(-SCENE3D_SIZE, 0.0f, -SCENE3D_SIZE);
	pVtx[1].pos = D3DXVECTOR3(-SCENE3D_SIZE, 0.0f ,SCENE3D_SIZE);
	pVtx[2].pos = D3DXVECTOR3(SCENE3D_SIZE, 0.0f, -SCENE3D_SIZE);
	pVtx[3].pos = D3DXVECTOR3(SCENE3D_SIZE, 0.0f, SCENE3D_SIZE);
	
	vec0 = pVtx[3].pos - pVtx[2].pos;
	vec1 = pVtx[3].pos - pVtx[1].pos;

	D3DXVec3Cross(&nor0, &vec0, &vec1);
	D3DXVec3Normalize(&nor0, &nor0);	// 正規化する

	pVtx[3].nor = nor1;

	vec0 = pVtx[0].pos - pVtx[1].pos;
	vec1 = pVtx[0].pos - pVtx[2].pos;

	D3DXVec3Cross(&nor1, &vec0, &vec1);
	D3DXVec3Normalize(&nor1, &nor1);	// 正規化する

	pVtx[0].nor = nor0;

	// 法線の設定
	pVtx[1].nor = (nor0 + nor1) / 2;;
	pVtx[2].nor = (nor0 + nor1) / 2;;
	
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
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor0, nor1;

	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	vec0 = pVtx[3].pos - pVtx[2].pos;
	vec1 = pVtx[3].pos - pVtx[1].pos;

	D3DXVec3Cross(&nor0, &vec0, &vec1);
	D3DXVec3Normalize(&nor0, &nor0);	// 正規化する

	pVtx[3].nor = nor0;

	vec0 = pVtx[0].pos - pVtx[1].pos;
	vec1 = pVtx[0].pos - pVtx[2].pos;

	D3DXVec3Cross(&nor1, &vec0, &vec1);
	D3DXVec3Normalize(&nor1, &nor1);	// 正規化する

	pVtx[0].nor = nor1;

	// 法線の設定
	pVtx[1].nor = (nor0 + nor1) / 2;;
	pVtx[2].nor = (nor0 + nor1) / 2;;

	CDebugProc::Print(1, "pVtx[0] %.1f,%.1f,%.1f\n", pVtx[0].nor.x, pVtx[0].nor.y, pVtx[0].nor.z);
	CDebugProc::Print(1, "pVtx[1] %.1f,%.1f,%.1f\n", pVtx[1].nor.x, pVtx[1].nor.y, pVtx[1].nor.z);
	CDebugProc::Print(1, "pVtx[2] %.1f,%.1f,%.1f\n", pVtx[2].nor.x, pVtx[2].nor.y, pVtx[2].nor.z);
	CDebugProc::Print(1, "pVtx[3] %.1f,%.1f,%.1f\n\n", pVtx[3].nor.x, pVtx[3].nor.y, pVtx[3].nor.z);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// 生成
//=============================================================================
CScene3D * CScene3D::Create(D3DXVECTOR3 pos)
{
	CScene3D *pScene3D = NULL;

	if (pScene3D == NULL)
	{
		pScene3D = new CScene3D;

		if (pScene3D != NULL)
		{
			pScene3D->Init(pos);
		}
	}
	return pScene3D;
}
//=============================================================================
// 位置の設置
//=============================================================================
void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置
	pVtx[0].pos = D3DXVECTOR3(-SCENE3D_SIZE, 0.0f,  +SCENE3D_SIZE);
	pVtx[1].pos = D3DXVECTOR3(+SCENE3D_SIZE, 0.0f,  +SCENE3D_SIZE);
	pVtx[2].pos = D3DXVECTOR3( -SCENE3D_SIZE, 0.0f,  -SCENE3D_SIZE);
	pVtx[3].pos = D3DXVECTOR3( +SCENE3D_SIZE, 0.0f,  -SCENE3D_SIZE);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CScene3D::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 頂点バッファの取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene3D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// 高さ取得
//=============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	float fGaiseki;	// 外積

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_vecA = pVtx[1].pos - pVtx[2].pos;
	m_vecC = pos - pVtx[2].pos;

	fGaiseki = (m_vecA.z *  m_vecC.x) - (m_vecA.x * m_vecC.z);

	if (fGaiseki >= 0)
	{
		pos.y = ((-pVtx[3].nor.x * (pos.x - pVtx[3].pos.x)) + (-pVtx[3].nor.z * (pos.z - pVtx[3].pos.z)) / pVtx[3].nor.y) + pVtx[3].pos.y;
	}

	m_vecA = pVtx[2].pos - pVtx[1].pos;
	m_vecC = pos - pVtx[1].pos;

	fGaiseki = (m_vecA.z *  m_vecC.x) - (m_vecA.x * m_vecC.z);

	if (fGaiseki > 0)
	{
		pos.y = ((-pVtx[0].nor.x * (pos.x - pVtx[0].pos.x)) + (-pVtx[0].nor.z * (pos.z - pVtx[0].pos.z)) / pVtx[0].nor.y) + pVtx[0].pos.y;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return pos.y;
}



