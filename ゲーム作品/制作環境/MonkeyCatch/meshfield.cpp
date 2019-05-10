//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "meshfield.h"
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
CMeshField::CMeshField()
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;		// インデックスバッファのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	// 各初期化の設定
	m_nNumVertex = VERTEX_KAZU;		// 頂点数
	m_nNumIndex = INDEX_KAZU;		// インデックス数
	m_nNumPolygon = POLYGON_KAZU;	// ポリゴン数

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, MF_TEXTURE, &m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_KAZU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_KAZU,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 2バイトのデータ
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
	{// 縦の面の数だけ繰り返し
		for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
		{// 横の面の数だけ繰り返し
			pVtx[0].pos = D3DXVECTOR3((nCntX * MESHFILED_POS) / MAX_MF_X, 0.0f, (nCntZ * -MESHFILED_POS) / MAX_MF_Z);		// rand()% 30 * 1.0f
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	CMeshField::Nor();

	WORD * pIdx;	// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0,nIdx = 0; nCntZ < MAX_MF_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++,nIdx++)
		{
			pIdx[0] = (MAX_MF_X + 1) + nIdx;
			pIdx[1] = 0 + nIdx;

			pIdx += 2;

			if (nCntZ < MAX_MF_Z - 1 && nCntX == MAX_MF_X)
			{
				pIdx[0] = 0 + nIdx;
				pIdx[1] = (MAX_MF_X + 1) + nIdx + 1;

				pIdx += 2;
			}
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
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
void CMeshField::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputKeyboard->GetPress(DIK_L) == true)
	{// 書き出し
		SaveMeshFiled();
	}

	if (pInputKeyboard->GetPress(DIK_K) == true)
	{// 読み込み
		LoadMeshFiled();
	}

	if (pInputKeyboard->GetPress(DIK_M) == true)
	{// 法線
		Nor();
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
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

	// インデックスデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_KAZU, 0, POLYGON_KAZU);
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CMeshField::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 高さ取得
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0, vec1, vec2;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 aVtxpos[3];

	D3DXVECTOR3 posmtx = pos - m_pos;

	int nNumber;	// 三角の参照する頂点
	float fHeight;	// プレイヤーの高さ保管

	// 現在何ブロックにいるか
	int nMeshX = (int)(posmtx.x / (MESHFILED_POS / MAX_MF_X));
	int nMeshZ = (int)(posmtx.z / (MESHFILED_POS / MAX_MF_Z) * -1);

	// 現在乗ってる頂点の出し方
	int nMeshLU = nMeshX + nMeshZ *(MAX_MF_X + 1);
	int nMeshRU = (nMeshX + 1) + nMeshZ * (MAX_MF_X + 1);
	int nMeshLD = nMeshX + (nMeshZ + 1) * (MAX_MF_X + 1);
	int nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (MAX_MF_X + 1);

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	////デバック用
	//CDebugProc::Print(1, "左上頂点 nMeshLU : %d\n", nMeshLU);
	//CDebugProc::Print(1, "右上頂点 nMeshRU : %d\n", nMeshRU);
	//CDebugProc::Print(1, "左下頂点 nMeshLD : %d\n", nMeshLD);
	//CDebugProc::Print(1, "右下頂点 nMeshRD : %d\n", nMeshRD);
	//CDebugProc::Print(1, "現在のブロック 横に %d ブロック目\n", nMeshX);
	//CDebugProc::Print(1, "現在のブロック 縦に %d ブロック目\n", nMeshZ);

	// ベクトルの計算式 (目的 - 現在)
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;

	// ベクトルの計算式 (自分の位置 - 現在)
	vec1 = posmtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{
		//CDebugProc::Print(1, "MeshField△右範囲 : TURE\n\n");

		vec0 = posmtx - pVtx[nMeshLU].pos;

		aVtxpos[0] = pVtx[nMeshRU].pos;
		aVtxpos[1] = pVtx[nMeshRD].pos;
		aVtxpos[2] = pVtx[nMeshLU].pos;

		nNumber = 3;
	}
	else if ((vec0.x *  vec1.z) - (vec0.z * vec1.x) >= 0)
	{
		//CDebugProc::Print(1, "MeshField△左範囲 : TURE\n\n");

		vec0 = posmtx - pVtx[nMeshRD].pos;

		aVtxpos[0] = pVtx[nMeshLD].pos;
		aVtxpos[1] = pVtx[nMeshLU].pos;
		aVtxpos[2] = pVtx[nMeshRD].pos;

		nNumber = 0;
	}

	fHeight = aVtxpos[0].y;

	aVtxpos[2].y -= aVtxpos[0].y;
	aVtxpos[1].y -= aVtxpos[0].y;
	aVtxpos[0].y -= aVtxpos[0].y;

	// ベクトルを出す式
	vec0 = aVtxpos[1] - aVtxpos[0];
	vec1 = aVtxpos[2] - aVtxpos[0];

	// 外積の式
	D3DXVec3Cross(&nor, &vec0, &vec1);

	// 正規化
	D3DXVec3Normalize(&nor, &nor);

	vec0 = posmtx - aVtxpos[0];

	// 内積の式
	((vec0.x * pVtx[nNumber].nor.x) + (vec0.y *  pVtx[nNumber].nor.y) + (vec0.z *  pVtx[nNumber].nor.z));
	vec0.y = (-(vec0.x * nor.x) - (vec0.z * nor.z)) / nor.y;

	// プレイヤーの位置を元に戻す
	posmtx.y = vec0.y + fHeight + m_pos.y;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return posmtx.y;
}
//=============================================================================
// 法線
//=============================================================================
void CMeshField::Nor(void)
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 aVtxNor[2];
	int nCntZ;
	int nCntX;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntZ = 0; nCntZ < MAX_MF_Z; nCntZ++)
	{// 縦の面の数だけ繰り返し
		for (nCntX = 0; nCntX < MAX_MF_X; nCntX++)
		{// 横の面の数だけ繰り返し
			pVtx[nCntX + nCntZ * (MAX_MF_X + 1)].nor = D3DXVECTOR3(0.0f, -0.3f, 0.0f);
		}
	}
	for (nCntZ = 0; nCntZ < MAX_MF_Z; nCntZ++)
	{// 縦の面の数だけ繰り返し
		for (nCntX = 0; nCntX < MAX_MF_X; nCntX++)
		{// 横の面の数だけ繰り返し

			int nMeshLU = nCntX + nCntZ *(MAX_MF_X + 1);
			int nMeshRU = (nCntX + 1) + nCntZ * (MAX_MF_X + 1);
			int nMeshLD = nCntX + (nCntZ + 1) * (MAX_MF_X + 1);
			int nMeshRD = (nCntX + 1) + (nCntZ + 1) * (MAX_MF_X + 1);

			// ベクトルの計算式 (目的 - 現在)
			vec0 = pVtx[nMeshLU].pos - pVtx[nMeshLD].pos;

			// ベクトルの計算式 (目的 - 現在)
			vec1 = pVtx[nMeshRD].pos - pVtx[nMeshLD].pos;

			// 外積の式
			D3DXVec3Cross(&aVtxNor[0], &vec0, &vec1);

			// 正規化
			D3DXVec3Normalize(&aVtxNor[0], &aVtxNor[0]);

			// ベクトルの計算式 (目的 - 現在)
			vec0 = pVtx[nMeshRD].pos - pVtx[nMeshRU].pos;

			// ベクトルの計算式 (目的 - 現在)
			vec1 = pVtx[nMeshLU].pos - pVtx[nMeshRU].pos;

			// 外積の式
			D3DXVec3Cross(&aVtxNor[1], &vec0, &vec1);

			// 正規化
			D3DXVec3Normalize(&aVtxNor[1], &aVtxNor[1]);

			pVtx[nMeshLD].nor += aVtxNor[0];
			pVtx[nMeshLU].nor += aVtxNor[0] + aVtxNor[1];
			pVtx[nMeshRD].nor += aVtxNor[0] + aVtxNor[1];
			pVtx[nMeshRU].nor += aVtxNor[1];
		}
	}

	for (nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
	{// 縦の面の数だけ繰り返し
		for (nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
		{// 横の面の数だけ繰り返し
			if (nCntZ == 0)
			{// 上端
				if (nCntX > 0 && nCntX < MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 3.0f;
				}
				else if (nCntX == 0)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 2.0f;
				}
			}
			else if (nCntZ == MAX_MF_Z)
			{// 下端
				if (nCntX > 0 && nCntX < MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 3.0f;
				}
				else if (nCntX == MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 2.0f;
				}
			}
			else
			{// 中央
				if (nCntX > 0 && nCntX < MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 6.0f;
				}
				else
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 3.0f;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 高さ設置
//=============================================================================
void CMeshField::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	D3DXVECTOR3 posmtx = pos - m_pos;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
	{// 縦の面の数だけ繰り返し
		for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
		{// 横の面の数だけ繰り返し

			float fLength = (float)sqrt((posmtx.x - pVtx->pos.x)*(posmtx.x - pVtx->pos.x) + (posmtx.z - pVtx->pos.z)*(posmtx.z - pVtx->pos.z));

			if (fLength <= fRange)
			{
				float fHeight = cosf((D3DX_PI * 0.5f)*(fLength / fRange)) * fValue;

				pVtx->pos.y += fHeight;
			}
			pVtx++;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 生成
//=============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos)
{
	CMeshField *pMeshFiled = NULL;

	if (pMeshFiled == NULL)
	{
		pMeshFiled = new CMeshField;

		if (pMeshFiled != NULL)
		{
			pMeshFiled->Init(pos);
		}
	}
	return pMeshFiled;
}
//=============================================================================
// セーブ
//=============================================================================
void CMeshField::SaveMeshFiled(void)
{
	FILE *pFile;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 書き出し
	pFile = fopen("data/TEXT/MeshfieldSave.txt", "w");
	if (pFile != NULL)
	{
		for (int nCntZ = 0; nCntZ < MAX_MF_Z; nCntZ++)
		{// 縦の面の数だけ繰り返し
			for (int nCntX = 0; nCntX < MAX_MF_X; nCntX++)
			{// 横の面の数だけ繰り返し
				fprintf(pFile, "%.1f\n",pVtx->pos.y);
				pVtx++;
			}
		}
		fclose(pFile);
	}
	else
	{
		printf("ファイルが開けません\n");
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ロード
//=============================================================================
void CMeshField::LoadMeshFiled(void)
{
	FILE *pFile;

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 読み込み
	pFile = fopen("data/TEXT/MeshfieldSave.txt", "r");
	if (pFile != NULL)
	{
		for (int nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
		{// 縦の面の数だけ繰り返し
			for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
			{// 横の面の数だけ繰り返し
				fscanf(pFile, "%f\n", &pVtx->pos.y);
				pVtx++;
			}
		}
		fclose(pFile);
	}
	else
	{
		printf("ファイルが開けません\n");
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}