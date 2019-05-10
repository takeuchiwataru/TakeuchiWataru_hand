//=============================================================================
//
// 草処理 [grass.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "grass.h"
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"

#include "scene3D.h"
#include "meshfield.h"
#include "meshorbit.h"
#include "object.h"
#include "particle.h"
#include "game.h"
#include "player.h"
#include "remain.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CGrass::m_pTexture = NULL;
LPD3DXMESH CGrass::m_apMesh[MAX_GRASS_MODEL] = {};
LPD3DXBUFFER CGrass::m_apBuffMat[MAX_GRASS_MODEL] = {};
DWORD CGrass::m_anNumMat[MAX_GRASS_MODEL] = {};
CMeshOrbit * CGrass::m_pMeshOrbit = NULL;
CObject * CGrass::m_pObject = NULL;
CPlayer * CGrass::m_pPlayer = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGrass::CGrass()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = GRASSSTATE_NONE;

	m_pKeyInfo = NULL;
	m_nKey = 0;
	m_nCouterMotion = 0;
	m_nNumParts = 0;
	m_StateSave = 0;

	for (int nCnt = 0; nCnt < MAX_GRASS_MODEL; nCnt++)
	{
		m_apModel[nCnt] = NULL;
		m_aIndexParent[nCnt] = 0;
		m_aPosOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_type = GRASSTYPE_000;
	m_bHit = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGrass::~CGrass()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGrass::Init(D3DXVECTOR3 pos)
{
	// モーションの読み込み
	LoadMotion();

	// オブジェクトのタイプの設定
	SetObjType(CScene::OBJTYPE_GRASS);

	// 情報の初期化
	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = GRASSSTATE_NONE;
	m_bHit = false;

	m_nKey = 0;

	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);

	int nNumVtex;					// 頂点数
	DWORD sizeFvF;					// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;					// 頂点バッファへのポインタ

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		// 頂点数を取得
		nNumVtex = m_apMesh[nCntModel]->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(m_apMesh[nCntModel]->GetFVF());

		// 頂点バッファをロック
		m_apMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
			if (m_VtxMin.x >= vtx.x)
			{
				m_VtxMin.x = vtx.x;
			}
			if (m_VtxMin.y >= vtx.y)
			{
				m_VtxMin.y = vtx.y;
			}
			if (m_VtxMin.z >= vtx.z)
			{
				m_VtxMin.z = vtx.z;
			}

			if (m_VtxMax.x <= vtx.x)
			{
				m_VtxMax.x = vtx.x;
			}
			if (m_VtxMax.y <= vtx.y)
			{
				m_VtxMax.y = vtx.y;
			}
			if (m_VtxMax.z <= vtx.z)
			{
				m_VtxMax.z = vtx.z;
			}
			pVtxBuff += sizeFvF;
		}
		// 頂点バッファをアンロック
		m_apMesh[nCntModel]->UnlockVertexBuffer();
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGrass::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CGrass::Update(void)
{
	// 過去の位置を取得
	m_posold = m_pos;

	if (m_move.x < GRASS_MOVESTAND && m_move.x > -GRASS_MOVESTAND && m_move.z < GRASS_MOVESTAND && m_move.z > -GRASS_MOVESTAND)
	{// ニュートラル
		m_state = GRASSSTATE_NORMAL;
	}

	// モーションの更新
	UpdateMotion();

	CScene *pScene = NULL;

	pScene = CScene::GetTop(3);

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetNext();	// 遺言  /* 更新したときに消える可能性があるから */

		if (pScene->GetDeath() == false)
		{// 死亡フラグが立っていたら

			CScene::OBJTYPE objType;

			// オブジェクトの取得
			objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_OBJECT)
			{// オブジェクトだったら
				((CObject*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
			if (m_type == GRASSTYPE_001)
			{// 壊れるタイプだったら
				if (objType == CScene::OBJTYPE_PLAYER)
				{// プレイヤーの武器の判定
					bool bHit = ((CPlayer*)pScene)->AttackCollision(m_pos, GRASS_COLLISION);

					if (bHit == true)
					{
						m_bUse = true;
					}
				}
			}
		}
		// 次に進める
		pScene = pSceneNext;
	}

	if (m_bUse == true)
	{// 武器が草に当たったら
		for (int nCntParticle = 0; nCntParticle < G_MAX_PARTICLE; nCntParticle++)
		{// パーティクルの生成
			CParticle::Create(G_PARTICLE_CREATEPOS,
				D3DXVECTOR3(sinf(G_PARTICLE_AROUND) * G_PARTICLE_RAND, cosf(G_PARTICLE_AROUND) * G_PARTICLE_RAND, 0.0f),
				G_PARTICLE_LIFE, G_PARTICLE_RADIUS,CParticle::PARTICLETYPE_000);
		}
		Uninit();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CGrass::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}
}
//=============================================================================
// モーションテキストの読み込み
//=============================================================================
void CGrass::LoadMotion(void)
{
	FILE * pFile;
	char *pStrCur;						// 文字列の先頭ポインタ
	char aLine[MAX_CHAR];				// 文字列読み込み用
	char aStr[MAX_CHAR];				// 文字列抜き出し用
	int nWord;
	int nSpace;
	int nIndex;							// インデックス
	D3DXVECTOR3 aPos[MAX_GRASS_MODEL];	// 位置(パーツ分)
	D3DXVECTOR3 aRot[MAX_GRASS_MODEL];	// 向き(パーツ分)
	int nLoop;							// ループ
	int nCntMotion = 0;					// モーション

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ファイルオープン
	pFile = fopen(GRASS_FILE, "r");

	if (pFile != NULL)
	{// ファイルが開けたら
		while (1)
		{
			pStrCur = ReadLine(pFile, &aLine[0]);
			pStrCur = GetLineTop(&aLine[0]);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], NUM_MODEL, strlen(NUM_MODEL)) == 0)
			{// モデル数
				pStrCur += strlen(NUM_MODEL);	// 頭出し

				strcpy(&aStr[0], pStrCur);	// 対象の文字列抜き出し

				m_nNumParts = atoi(pStrCur);	// 文字列を値に変換

				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{// モデル数分繰り返し
					pStrCur = ReadLine(pFile, &aLine[0]);
					pStrCur = GetLineTop(&aLine[0]);
					strcpy(&aStr[0], pStrCur);

					if (memcmp(&aStr[0], MODEL_FILENAME, strlen(MODEL_FILENAME)) == 0)
					{// モデルファイル
						pStrCur += strlen(MODEL_FILENAME);	// 頭出し
						strcpy(&aStr[0], pStrCur);

						// コメント消す
						nSpace = PopString(pStrCur, &aStr[0]);
						strcpy(&aStr[0], pStrCur);
						aStr[nSpace - 1] = '\0';

						strcpy(&m_aFilenameModel[nCntModel][0], aStr);	// 対象の文字列抜き出し

						// Xファイルの読み込み
						D3DXLoadMeshFromX(&m_aFilenameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_apBuffMat[nCntModel],
							NULL,
							&m_anNumMat[nCntModel],
							&m_apMesh[nCntModel]);
					}
				}
			}
			if (memcmp(&aStr[0], CHARACTERSET, strlen(CHARACTERSET)) == 0)
			{// キャラクター情報

			 // 1行読み込み
				pStrCur = ReadLine(pFile, &aLine[0]);
				pStrCur = GetLineTop(&aLine[0]);

				strcpy(&aStr[0], pStrCur);

				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{
					if (memcmp(&aStr[0], NUM_PARTS, strlen(NUM_PARTS)) == 0)
					{// パーツ数
						pStrCur += strlen(NUM_PARTS);	// 頭出し

						strcpy(&aStr[0], pStrCur);	// 対象の文字列抜き出し

						m_nNumParts = atoi(pStrCur);	// 文字列を値に変換	

														// 1行読み込み
						pStrCur = ReadLine(pFile, &aLine[0]);
						pStrCur = GetLineTop(&aLine[0]);

						strcpy(&aStr[0], pStrCur);
					}

					if (memcmp(&aStr[0], PARTSSET, strlen(PARTSSET)) == 0)
					{// パーツ情報

						// 1行読み込み
						pStrCur = ReadLine(pFile, &aLine[0]);
						pStrCur = GetLineTop(&aLine[0]);

						strcpy(&aStr[0], pStrCur);

						if (memcmp(&aStr[0], INDEX, strlen(INDEX)) == 0)
						{// インデックス

							pStrCur += strlen(INDEX);	// 頭出し

							strcpy(&aStr[0], pStrCur);	// 対象の文字列抜き出し

							nIndex = atoi(pStrCur);	// 文字列を値に変換

							// 1行読み込み
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], PARENT, strlen(PARENT)) == 0)
						{// 親子関係
							pStrCur += strlen(PARENT);	// 頭出し

							strcpy(&aStr[0], pStrCur);	// 対象の文字列抜き出し

							m_aIndexParent[nIndex] = atoi(pStrCur);	// 文字列を値に変換

							// 1行読み込み
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], POS, strlen(POS)) == 0)
						{// 位置
							pStrCur += strlen(POS);		// 頭出し

							aPos[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							aPos[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							aPos[nIndex].z = (float)atof(pStrCur);	// Z

							// 1行読み込み
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], ROT, strlen(ROT)) == 0)
						{// 向き
							pStrCur += strlen(ROT);		// 頭出し

							aRot[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							aRot[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							aRot[nIndex].z = (float)atof(pStrCur);	// Z

							// 1行読み込み
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}
						if (memcmp(&aStr[0], END_PARTSSET, strlen(END_PARTSSET)) == 0)
						{
							// 1行読み込み
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (m_aIndexParent[nIndex] == -1)
						{// 親
							m_apModel[0] = CModel::Create(D3DXVECTOR3(m_pos.x + aPos[nIndex].x, m_pos.y + aPos[nIndex].y, aPos[nIndex].z));
							m_apModel[0]->BindModel(m_apMesh[nIndex], m_apBuffMat[nIndex], m_anNumMat[nIndex]);
							m_apModel[0]->SetParent(NULL);			// 体

							// オフセットに代入
							m_aPosOffset[0] = m_apModel[0]->GetPos();
						}
						else if (m_aIndexParent[nIndex] > -1)
						{// 子
							m_apModel[nIndex] = CModel::Create(D3DXVECTOR3(aPos[nIndex].x, aPos[nIndex].y, aPos[nIndex].z));
							m_apModel[nIndex]->BindModel(m_apMesh[nIndex], m_apBuffMat[nIndex], m_anNumMat[nIndex]);
							m_apModel[nIndex]->SetParent(m_apModel[m_aIndexParent[nIndex]]);

							// オフセットに代入
							m_aPosOffset[nIndex] = m_apModel[nIndex]->GetPos();
						}
					}
				}
			}
			if (memcmp(&aStr[0], END_CHARACTERSET, strlen(END_CHARACTERSET)) == 0)
			{
				// 1行読み込み
				pStrCur = ReadLine(pFile, &aLine[0]);
				pStrCur = GetLineTop(&aLine[0]);

				strcpy(&aStr[0], pStrCur);
			}
			if (memcmp(&aStr[0], MOTIONSET, strlen(MOTIONSET)) == 0)
			{// モーション情報
				while (1)
				{
					// 1行読み込み
					pStrCur = ReadLine(pFile, &aLine[0]);
					pStrCur = GetLineTop(&aLine[0]);

					strcpy(&aStr[0], pStrCur);

					if (memcmp(&aStr[0], LOOP, strlen(LOOP)) == 0)
					{// ループするかどうか
						pStrCur += strlen(LOOP);	// 頭出し

						strcpy(&aStr[0], pStrCur);	// 対象の文字列抜き出し

						nLoop = atoi(pStrCur);	// 文字列を値に変換	

						if (nLoop == 1)
						{// 1だったらループする
							m_aMotionInfo[nCntMotion].bLoop = true;
						}
						else if (nLoop == 0)
						{// 0だったらループしない
							m_aMotionInfo[nCntMotion].bLoop = false;
						}
					}
					if (memcmp(&aStr[0], NUM_KEY, strlen(NUM_KEY)) == 0)
					{// キー数
						pStrCur += strlen(NUM_KEY);	// 頭出し

						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrCur);

						// 1行読み込み
						pStrCur = ReadLine(pFile, &aLine[0]);
						pStrCur = GetLineTop(&aLine[0]);

						strcpy(&aStr[0], pStrCur);

						for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey; nCntKey++)
						{// キー数分繰り返し
							if (memcmp(&aStr[0], KEYSET, strlen(KEYSET)) == 0)
							{// キー情報始め

								// 1行読み込み
								pStrCur = ReadLine(pFile, &aLine[0]);
								pStrCur = GetLineTop(&aLine[0]);

								strcpy(&aStr[0], pStrCur);

								if (memcmp(&aStr[0], FRAME, strlen(FRAME)) == 0)
								{// フレーム数
									pStrCur += strlen(FRAME);	// 頭出し

									m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(pStrCur);
								}

								// 1行読み込み
								pStrCur = ReadLine(pFile, &aLine[0]);
								pStrCur = GetLineTop(&aLine[0]);

								strcpy(&aStr[0], pStrCur);

								for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
								{// モデル数分の繰り返し
									if (memcmp(&aStr[0], KEY, strlen(KEY)) == 0)
									{// キー情報

									 // 1行読み込み
										pStrCur = ReadLine(pFile, &aLine[0]);
										pStrCur = GetLineTop(&aLine[0]);

										strcpy(&aStr[0], pStrCur);

										if (memcmp(&aStr[0], POS, strlen(POS)) == 0)
										{// 位置
											pStrCur += strlen(POS);		// 頭出し

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosX = (float)atof(pStrCur);	// X

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// 次まで進める

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosY = (float)atof(pStrCur);	// Y

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// 次まで進める

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosZ = (float)atof(pStrCur);	// Z

																																		// 1行読み込み
											pStrCur = ReadLine(pFile, &aLine[0]);
											pStrCur = GetLineTop(&aLine[0]);

											strcpy(&aStr[0], pStrCur);
										}

										if (memcmp(&aStr[0], ROT, strlen(ROT)) == 0)
										{// 向き
											pStrCur += strlen(ROT);		// 頭出し

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotX = (float)atof(pStrCur);	// X

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// 次まで進める

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotY = (float)atof(pStrCur);	// Y

											nWord = PopString(pStrCur, &aStr[0]);

											pStrCur += nWord;	// 次まで進める

											m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotZ = (float)atof(pStrCur);	// Z

																																		// 1行読み込み
											pStrCur = ReadLine(pFile, &aLine[0]);
											pStrCur = GetLineTop(&aLine[0]);

											strcpy(&aStr[0], pStrCur);
										}
										if (memcmp(&aStr[0], END_KEY, strlen(END_KEY)) == 0)
										{
											// 1行読み込み
											pStrCur = ReadLine(pFile, &aLine[0]);
											pStrCur = GetLineTop(&aLine[0]);

											strcpy(&aStr[0], pStrCur);
										}
									}
								}
							}
							if (memcmp(&aStr[0], END_KEYSET, strlen(END_KEYSET)) == 0)
							{// キー情報終わり
								// 1行読み込み
								pStrCur = ReadLine(pFile, &aLine[0]);
								pStrCur = GetLineTop(&aLine[0]);

								strcpy(&aStr[0], pStrCur);
							}
						}
					}
					if (memcmp(&aStr[0], END_MOTIONSET, strlen(END_MOTIONSET)) == 0)
					{
						m_pKeyInfo = &m_aMotionInfo[nCntMotion].aKeyInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			if (memcmp(&aStr[0], "END_SCRIPT", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
}
//=============================================================================
// モーションの更新
//=============================================================================
void CGrass::UpdateMotion(void)
{
	m_StateSave = m_state - 1;

	E_KEY *pKey, *pKeyNext;				// 現在のキー,次のキー
	float fRateMotion;					// 現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対比を算出
	float fDiffMotion;					// 現在のキーと次のキーの各要素の差分を値を算出
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rotMotion;

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// モデルのパーツ分繰り返し

		if (m_apModel[nCntModel] != NULL)
		{
			// 現在のキーを取得
			pKey = &m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].aKey[nCntModel];

			// 次のキーを取得
			pKeyNext = &m_aMotionInfo[m_StateSave].aKeyInfo[(m_nKey + 1) % m_aMotionInfo[m_StateSave].nNumKey].aKey[nCntModel];

			// 現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)(m_nCouterMotion) / (float)m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame;

			//==============
			//　POS
			//==============

			// 現在のキーと次のキーの各要素の差分を値を算出
			fDiffMotion = pKeyNext->fPosX - pKey->fPosX;
			// 相対値と差分を使って各要素の値を算出
			posMotion.x = pKey->fPosX + (fDiffMotion * fRateMotion);

			// 現在のキーと次のキーの各要素の差分を値を算出
			fDiffMotion = pKeyNext->fPosY - pKey->fPosY;
			// 相対値と差分を使って各要素の値を算出
			posMotion.y = pKey->fPosY + (fDiffMotion * fRateMotion);

			// 現在のキーと次のキーの各要素の差分を値を算出
			fDiffMotion = pKeyNext->fPosZ - pKey->fPosZ;
			// 相対値と差分を使って各要素の値を算出
			posMotion.z = pKey->fPosZ + (fDiffMotion * fRateMotion);

			//==============
			//　ROT
			//==============

			// 現在のキーと次のキーの各要素の差分を値を算出
			fDiffMotion = pKeyNext->fRotX - pKey->fRotX;
			// 相対値と差分を使って各要素の値を算出
			rotMotion.x = pKey->fRotX + (fDiffMotion * fRateMotion);

			// 現在のキーと次のキーの各要素の差分を値を算出
			fDiffMotion = pKeyNext->fRotY - pKey->fRotY;
			// 相対値と差分を使って各要素の値を算出
			rotMotion.y = pKey->fRotY + (fDiffMotion * fRateMotion);

			// 現在のキーと次のキーの各要素の差分を値を算出
			fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;
			// 相対値と差分を使って各要素の値を算出
			rotMotion.z = pKey->fRotZ + (fDiffMotion * fRateMotion);

			// 位置
			m_apModel[nCntModel]->SetPos(m_aPosOffset[nCntModel] + posMotion);

			// 向き
			m_apModel[nCntModel]->SetRot(rotMotion);
		}
	}
	m_nCouterMotion++;

	if (m_aMotionInfo[m_StateSave].bLoop == true)
	{//　ループする
		if (m_nCouterMotion >= m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame)
		{
			if (m_nKey >= m_aMotionInfo[m_StateSave].nNumKey - 1)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey = 1;
			}
			m_nCouterMotion = 0;
		}
	}
	else
	{// ループしない
		if (m_nCouterMotion > m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame)
		{
			if (m_nKey < m_aMotionInfo[m_StateSave].nNumKey - 1)
			{
				m_nKey++;
			}
			else if (m_nKey >= m_aMotionInfo[m_StateSave].nNumKey)
			{
				m_nKey = 0;
			}

			m_nCouterMotion = 0;
		}
	}
}
//=============================================================================
// 無効な行無視
//=============================================================================
char *CGrass::ReadLine(FILE * pFile, char *pDst)
{
	bool bChar = true;
	while (bChar == true)
	{
		fgets(pDst, MAX_CHAR, pFile);

		if (pDst[0] == '#')
		{// コメントだった場合

		}
		else if (pDst[0] == '\n')
		{// 改行だった場合

		}
		else if (pDst[0] == '\t')
		{
			for (int nCntChar = 0; nCntChar < MAX_CHAR; nCntChar++)
			{
				if (pDst[nCntChar + 1] == '#')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\n')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\t')
				{

				}
				else if (pDst[nCntChar + 1] == ' ')
				{

				}
				else
				{
					bChar = false;
					break;
				}
			}
		}
		else if (pDst[0] == ' ')
		{
			for (int nCntChar = 0; nCntChar < MAX_CHAR; nCntChar++)
			{
				if (pDst[nCntChar + 1] == '#')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\n')
				{
					break;
				}
				else if (pDst[nCntChar + 1] == '\t')
				{

				}
				else if (pDst[nCntChar + 1] == ' ')
				{

				}
				else
				{
					bChar = false;
					break;
				}
			}
		}
		else
		{
			bChar = false;
		}
	}
	return pDst;
}
//=============================================================================
// tabとかを飛ばす
//=============================================================================
char *CGrass::GetLineTop(char *pSrc)
{
	while (1)
	{
		if (memcmp(&pSrc[0], "\t", strlen("\t")) == 0)
		{// タブだった場合
			pSrc += strlen("\t");
		}
		else if (memcmp(&pSrc[0], " ", strlen(" ")) == 0)
		{// スペースだった場合
			pSrc += strlen(" ");
		}
		else
		{
			break;
		}
	}
	return pSrc;
}
//=============================================================================
// 
//=============================================================================
int CGrass::PopString(char * pSrc, char * pDst)
{
	int nCount = 0;
	while (1)
	{
		pSrc++;
		nCount++;

		if (memcmp(pSrc, " ", strlen(" ")) == 0)
		{
			pSrc = "\0";
			nCount++;
			break;
		}
		else if (memcmp(pSrc, "\t", strlen("\t")) == 0)
		{
			pSrc = "\0";
			nCount++;
			break;
		}
	}
	strcpy(pDst, pSrc);

	return nCount;
}
//=============================================================================
// 当たり判定
//=============================================================================
bool CGrass::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin)
{
	m_bHit = false;		// 当たったかどうか

	if (pPos->y + pVtxMax->y > m_pos.y + m_VtxMin.y && pPos->y + pVtxMin->y < m_pos.y + m_VtxMax.y)
	{// モデルの高さ分
		if (pPos->x + pVtxMax->x > m_pos.x + m_VtxMin.x && pPos->x + pVtxMin->x < m_pos.x + m_VtxMax.x)
		{// Z面の判定
			if (pPosold->z + pVtxMin->z >= m_pos.z + m_VtxMax.z && m_pos.z + m_VtxMax.z >= pPos->z + pVtxMin->z)
			{// 奧の判定
				pPos->z = pPosold->z;
				m_bHit = true;
			}
			if (pPosold->z + pVtxMax->z <= m_pos.z + m_VtxMin.z && m_pos.z + m_VtxMin.z <= pPos->z + pVtxMax->z)
			{// 手前の判定
				pPos->z = pPosold->z;
				m_bHit = true;
			}
		}

		if (pPos->z + pVtxMax->z > m_pos.z + m_VtxMin.z && pPos->z + pVtxMin->z < m_pos.z + m_VtxMax.z)
		{// X面の判定
			if (pPosold->x + pVtxMin->x >= m_pos.x + m_VtxMax.x && m_pos.x + m_VtxMax.x >= pPos->x + pVtxMin->x)
			{// 右面の判定		
				pPos->x = pPosold->x;
				m_bHit = true;
			}
			if (pPosold->x + pVtxMax->x <= m_pos.x + m_VtxMin.x && m_pos.x + m_VtxMin.x <= pPos->x + pVtxMax->x)
			{// 左面の判定
				pPos->x = pPosold->x;
				m_bHit = true;
			}
		}
	}
	return m_bHit;
}
//=============================================================================
// 草の生成
//=============================================================================
CGrass * CGrass::Create(D3DXVECTOR3 pos, GRASSTYPE type)
{
	CGrass *pGrass = NULL;

	if (pGrass == NULL)
	{
		pGrass = new CGrass;

		if (pGrass != NULL)
		{
			pGrass->Init(pos);
			pGrass->SetType(type);	// 種類
		}
	}
	return pGrass;
}
//=============================================================================
// 草の種類設定
//=============================================================================
void CGrass::SetType(GRASSTYPE type)
{
	m_type = type;
}
