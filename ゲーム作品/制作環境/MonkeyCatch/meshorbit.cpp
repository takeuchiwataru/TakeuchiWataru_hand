//=============================================================================
//
// メッシュオービット処理 [meshorbit.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "meshorbit.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CPlayer *CMeshOrbit::m_pPlayer = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshOrbit::CMeshOrbit():CScene(6)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nNumPos = 0;
	m_nNumPolygon = 0;
	m_fCol = 0.0f;
	m_bUse = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshOrbit::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fCol = 0.0f;

	for (int nCount = 0; nCount < MAX_VERTEX; nCount++)
	{// 頂点数分繰り返し

		// 法線の設定
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色
		pVtx[nCount].col = D3DXCOLOR(0.0f, 0.5f, 0.8f, m_fCol -(nCount*0.05f));
	}

	// メッシュオービットのオフセット
	m_aOffset[0] = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_aOffset[1] = D3DXVECTOR3(0.0f, 40.0f, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CMeshOrbit::Uninit(void)
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

	if (m_pPlayer != NULL)
	{// プレイヤーがNULLじゃなかったらNULLにする
		m_pPlayer = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CMeshOrbit::Update(void)
{
	m_fCol += ORBIT_COLOR;
	if (m_fCol >= 1.0f)
	{
		m_fCol = 1.0f;
	}
	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCount = 0; nCount < MAX_VERTEX; nCount++)
	{// 頂点数分繰り返し

	 // 法線の設定
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色
		pVtx[nCount].col = D3DXCOLOR(0.0f, 0.5f, 0.8f, m_fCol - (nCount*0.05f));
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void CMeshOrbit::Draw(void)
{
	// プレイヤーの取得
	m_pPlayer = CGame::GetPlayer();

	if (m_pPlayer != NULL)
	{// プレイヤーがNULLじゃなかったら
		if (m_pPlayer->GetState() == m_pPlayer->PLAYERSTATE_MUSIAMI)
		{// プレイヤーの状態が虫網状態だったら
			D3DXMATRIX	mtxParent;					// 親のマトリックス

			CManager manager;	//インスタンス

			 //頂点情報を設定
			VERTEX_3D * pVtx;					// 頂点情報へのポインタ

			// デバイスの取得
			LPDIRECT3DDEVICE9 pDevice;
			pDevice = manager.GetRenderer()->GetDevice();

			// 頂点バッファをロック
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCount = MAX_VERTEX - 3; nCount >= 0; nCount--)
			{// 入れ替え
				m_aPosVertex[nCount + 2] = m_aPosVertex[nCount];

				pVtx[nCount + 2].pos = m_aPosVertex[nCount + 2];
			}

			// ベクトルをトランスフォームしてベクトルを得る
			D3DXVec3TransformCoord(&m_aPosVertex[0], &m_aOffset[0], m_pMtxParent);

			pVtx[0].pos = m_aPosVertex[0];

			// ベクトルをトランスフォームしてベクトルを得る
			D3DXVec3TransformCoord(&m_aPosVertex[1], &m_aOffset[1], m_pMtxParent);

			pVtx[1].pos = m_aPosVertex[1];

			// 頂点バッファをアンロック
			m_pVtxBuff->Unlock();

			// カリングON
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// 掛け合わせる
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxWorld);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_POLYGON);

			// カリングOFF
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
	}
}
//=============================================================================
// 生成
//=============================================================================
CMeshOrbit * CMeshOrbit::Create(D3DXVECTOR3 pos)
{
	CMeshOrbit *pMeshOrbit = NULL;

	if (pMeshOrbit == NULL)
	{
		pMeshOrbit = new CMeshOrbit;

		if (pMeshOrbit != NULL)
		{
			pMeshOrbit->Init(pos);
		}
	}
	return pMeshOrbit;
}
//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 親のマトリックスの設置
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtx)
{
	m_pMtxParent = pMtx;
}
//=============================================================================
// ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CMeshOrbit::GetMtxWorld(void)
{
	return m_mtxWorld;
}
