//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "enemy.h"
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
#include "grass.h"

#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "remain.h"
#include "score.h"
#include "timer.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL;
LPD3DXMESH CEnemy::m_apMesh[MAX_ENEMY_MODEL] = {};
LPD3DXBUFFER CEnemy::m_apBuffMat[MAX_ENEMY_MODEL] = {};
DWORD CEnemy::m_anNumMat[MAX_ENEMY_MODEL] = {};

CMeshOrbit * CEnemy::m_pMeshOrbit = NULL;
CObject * CEnemy::m_pObject = NULL;
CPlayer * CEnemy::m_pPlayer = NULL;
int CEnemy::m_nNumEnemy = 0;
int CEnemy::m_nGetEnemy = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = ENEMYSTATE_NONE;

	m_pKeyInfo = NULL;
	m_nKey = 0;
	m_nCouterMotion = 0;
	m_nNumParts = 0;
	m_StateSave = 0;

	for (int nCnt = 0; nCnt < MAX_ENEMY_MODEL; nCnt++)
	{
		m_apModel[nCnt] = NULL;
		m_aIndexParent[nCnt] = 0;
		m_aPosOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_bUse = false;
	m_bHit = false;
	m_bHitGrass = false;
	m_bHitObject = false;

	m_type = ENEMYTYPE_000;
	m_MoveCounter = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// モーションの読み込み
	LoadMotion();

	// オブジェクトのタイプの設定
	SetObjType(CScene::OBJTYPE_ENEMY);

	// 情報の初期化
	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = ENEMYSTATE_NONE;

	m_bJump = false;
	m_bHitGrass = false;
	m_bHitObject = false;

	m_nNumEnemy = NUM_ENEMY;	// 敵の総数
	m_nGetEnemy = 0;			// ゲットした敵の数
	m_nKey = 0;
	m_MoveCounter = 0;

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
void CEnemy::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		//m_apModel[nCntModel]->Uninit();

		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TUTORIAL)
	{// モードがチュートリアル
		UpdateTutorial();
	}

	if (mode == CManager::MODE_GAME)
	{
		// カメラの取得
		CCamera *pCamera = CGame::GetCamera();
		D3DXVECTOR3 rot = pCamera->GetCamera();

		// 過去の位置を取得
		m_posold = m_pos;

		m_pos.y -= m_move.y;
		m_move.y += ENEMY_GRAVITY;		// 重力加算

		// メッシュフィールドの取得
		CMeshField *pMeshField = CGame::GetMeshField();
		float fHight = pMeshField->GetHeight(m_pos);

		if (m_pos.y < fHight)
		{
			m_pos.y = fHight;
		}

		if (m_type == ENEMYTYPE_000)
		{// 隠れているやつ
			m_fDest = (D3DX_PI * 0.0f);
		}
		if (m_type == ENEMYTYPE_001)
		{// お試し用
			if (m_bHitGrass == false || m_bHitObject == false)
			{
				m_move.x += ENEMY_MOVE000;

				if (m_move.x <= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * 0.5f);
				}
				if (m_move.x >= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * -0.5f);
				}
			}

			if (m_bHitGrass == true || m_bHitObject == true)
			{// 草に当たったら
				m_MoveCounter++;

				m_move.z -= ENEMY_MOVE000;

				if (m_move.z >= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * 1.0f);
				}
				if (m_move.z <= ENEMY_MOVESTAND)
				{
					m_fDest = (D3DX_PI * 0.0f);
				}
				if (m_MoveCounter == ENEMY_COUNTER000)
				{
					m_bHitGrass = false;
				}
			}
		}

		if (m_type == ENEMYTYPE_002)
		{// 横
			m_move.x += ENEMY_MOVE000;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER001)
			{
				m_move.x *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.x >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * -0.5f);
			}
			if (m_move.x <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.5f);
			}
		}
		if (m_type == ENEMYTYPE_003)
		{// 縦
			m_move.z -= ENEMY_MOVE001;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER000)
			{
				m_move.z *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.z >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 1.0f);
			}
			if (m_move.z <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.0f);
			}
		}

		if (m_type == ENEMYTYPE_004)
		{// 横 ver2
			m_move.x -= ENEMY_MOVE000;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER001)
			{
				m_move.x *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.x >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * -0.5f);
			}
			if (m_move.x <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.5f);
			}
		}
		if (m_type == ENEMYTYPE_005)
		{// 縦 ver2
			m_move.z += ENEMY_MOVE001;

			m_MoveCounter++;

			if (m_MoveCounter >= ENEMY_COUNTER000)
			{
				m_move.z *= -1;

				m_MoveCounter = 0;
			}
			if (m_move.z >= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 1.0f);
			}
			if (m_move.z <= ENEMY_MOVESTAND)
			{
				m_fDest = (D3DX_PI * 0.0f);
			}
		}

		// 角度の設定
		SetAngle();

		if (m_move.x < ENEMY_MOVESTAND && m_move.x > -ENEMY_MOVESTAND && m_move.z < ENEMY_MOVESTAND && m_move.z > -ENEMY_MOVESTAND)
		{// ニュートラル
			m_state = ENEMYSTATE_NORMAL;
		}
		if (m_move.x >= ENEMY_MOVESTAND || m_move.x <= -ENEMY_MOVESTAND || m_move.z >= ENEMY_MOVESTAND || m_move.z <= -ENEMY_MOVESTAND)
		{// 移動
			m_state = ENEMYSTATE_MOVE;
		}

		// 位置更新
		m_pos.x += m_move.x;
		m_pos.z += m_move.z;

		// 減速
		m_move.x += (0.0f - m_move.x) * 0.02f;
		m_move.z += (0.0f - m_move.z) * 0.02f;

		// モーションの更新
		UpdateMotion();

		// 当たり判定全般
		CollisionAll();

		// マップ当たり判定
		MapCollision();

		// 残機の取得
		CRemain * pRemain = CGame::GetRemain();

		// スコアの取得
		CScore * pScore = CGame::GetScore();

		// タイマーの取得
		CTimer * pTimer = CGame::GetTimer();

		if (m_bUse == true)
		{// 捕まったら
			Uninit();

			for (int nCntParticle = 0; nCntParticle < E_MAX_PARTICLE; nCntParticle++)
			{// パーティクルの生成
				CParticle::Create(D3DXVECTOR3(E_PARTICLE_CREATEPOS),
					D3DXVECTOR3(sinf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), cosf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), 0.0f),
					E_PARTICLE_LIFE, E_PARTICLE_RADIUS, CParticle::PARTICLETYPE_000);
			}

			// 総数を減らす
			m_nNumEnemy--;

			// ゲットした数増やす
			m_nGetEnemy++;

			// 残機減らす
			pRemain->AddRemain(-ENEMY_REMAIN);

			// スコア加算
			pScore->AddScore(ENEMY_SCORE);
		}

		if (m_nNumEnemy == 0)
		{// 敵が0になったら

			// タイマーストップ
			pTimer->StopTimer();

			// フェードの取得
			CFade *pFade = CManager::GetFade();
			pFade->Set(CManager::MODE_RESULT, pFade->FADE_OUT);
		}
	}
}
//=============================================================================
// 更新処理(チュートリアル用)
//=============================================================================
void CEnemy::UpdateTutorial(void)
{
	// カメラの取得
	CCamera *pCamera = CTutorial::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetCamera();

	// 過去の位置を取得
	m_posold = m_pos;

	m_pos.y -= m_move.y;
	m_move.y += ENEMY_GRAVITY;		// 重力加算

	// メッシュフィールドの取得
	CMeshField *pMeshField = CTutorial::GetMeshField();
	float fHight = pMeshField->GetHeight(m_pos);

	if (m_pos.y < fHight)
	{
		m_pos.y = fHight;
	}

	if (m_type == ENEMYTYPE_000)
	{// 隠れているやつ
		m_fDest = (D3DX_PI * 0.0f);
	}

	// 角度の設定
	SetAngle();

	if (m_move.x < ENEMY_MOVESTAND && m_move.x > -ENEMY_MOVESTAND && m_move.z < ENEMY_MOVESTAND && m_move.z > -ENEMY_MOVESTAND)
	{// ニュートラル
		m_state = ENEMYSTATE_NORMAL;
	}
	if (m_move.x >= ENEMY_MOVESTAND || m_move.x <= -ENEMY_MOVESTAND || m_move.z >= ENEMY_MOVESTAND || m_move.z <= -ENEMY_MOVESTAND)
	{// 移動
		m_state = ENEMYSTATE_MOVE;
	}

	// 位置更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	// 減速
	m_move.x += (0.0f - m_move.x) * 0.02f;
	m_move.z += (0.0f - m_move.z) * 0.02f;

	// モーションの更新
	UpdateMotion();

	// 当たり判定全般
	CollisionAll();

	// マップ当たり判定
	MapCollision();

	// スコアの取得
	CScore * pScore = CTutorial::GetScore();

	if (m_bUse == true)
	{// 捕まったら
		Uninit();

		for (int nCntParticle = 0; nCntParticle < E_MAX_PARTICLE; nCntParticle++)
		{// パーティクルの生成
			CParticle::Create(D3DXVECTOR3(E_PARTICLE_CREATEPOS),
				D3DXVECTOR3(sinf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), cosf(E_PARTICLE_AROUND) * (E_PARTICLE_RAND), 0.0f),
				E_PARTICLE_LIFE, E_PARTICLE_RADIUS,CParticle::PARTICLETYPE_000);
		}

		// 総数を減らす
		m_nNumEnemy--;

		// ゲットした数増やす
		m_nGetEnemy++;

		// スコア加算
		pScore->AddScore(ENEMY_SCORE);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
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

	// 影の描画処理
	D3DXMATRIX	mtxShadow;
	D3DXPLANE	planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;

	// ライトを無効にする
	pDevice->LightEnable(0, FALSE);
	pDevice->LightEnable(1, FALSE);
	pDevice->LightEnable(2, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = ENEMY_VECLIGHT;
	pos = ENEMY_POS;
	normal = ENEMY_NORMAL;

	// 平面を作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// 平面に射影するマトリックスを作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}
	// 元の設定に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

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
void CEnemy::LoadMotion(void)
{
	FILE * pFile;
	char *pStrCur;						// 文字列の先頭ポインタ
	char aLine[MAX_CHAR];				// 文字列読み込み用
	char aStr[MAX_CHAR];				// 文字列抜き出し用
	int nWord;
	int nSpace;
	int nIndex;							// インデックス
	D3DXVECTOR3 aPos[MAX_ENEMY_MODEL];	// 位置(パーツ分)
	D3DXVECTOR3 aRot[MAX_ENEMY_MODEL];	// 向き(パーツ分)
	int nLoop;							// ループ
	int nCntMotion = 0;					// モーション

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ファイルオープン
	pFile = fopen(ENEMY_FILE, "r");

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
void CEnemy::UpdateMotion(void)
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
char *CEnemy::ReadLine(FILE * pFile, char *pDst)
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
char *CEnemy::GetLineTop(char *pSrc)
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
int CEnemy::PopString(char * pSrc, char * pDst)
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
// 角度の設定
//=============================================================================
void CEnemy::SetAngle(void)
{
	m_fDiff = m_fDest - m_rot.y;

	if (m_fDiff > D3DX_PI)
	{
		m_fDiff -= D3DX_PI* ENEMY_AROUND;
	}
	if (m_fDiff < -D3DX_PI)
	{
		m_fDiff += D3DX_PI* ENEMY_AROUND;
	}

	m_rot.y += m_fDiff * ENEMY_DIFF;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI* ENEMY_AROUND;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI* ENEMY_AROUND;
	}
}
//=============================================================================
// マップ当たり判定
//=============================================================================
void CEnemy::MapCollision(void)
{
	if (m_pos.z > -ENEMY_MIN_POS)
	{// Zの奧
		m_pos.z = -ENEMY_MIN_POS;
	}
	else if (m_pos.z < -ENEMY_MAX_POS)
	{// Zの手前
		m_pos.z = -ENEMY_MAX_POS;
	}
	else if (m_pos.x < ENEMY_MIN_POS)
	{// Xの左
		m_pos.x = ENEMY_MIN_POS;
	}
	else if (m_pos.x > ENEMY_MAX_POS)
	{// Xの右
		m_pos.x = ENEMY_MAX_POS;
	}
}
//=============================================================================
// 当たり判定全般
//=============================================================================
void CEnemy::CollisionAll(void)
{
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
				bool bObjectHit = ((CObject*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);

				if (bObjectHit == true)
				{// オブジェクトに当たったら
					m_bHitObject = bObjectHit;
				}
			}
			if (objType == CScene::OBJTYPE_PLAYER)
			{// プレイヤーの武器の判定
				bool bHit = ((CPlayer*)pScene)->AttackCollision(m_pos, ENEMY_COLLISION);

				if (bHit == true)
				{// 当たり判定ON
					m_bUse = true;
				}
			}
			if (objType == CScene::OBJTYPE_GRASS)
			{// 草だったら
				bool bGrassHit = ((CGrass*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);

				if (bGrassHit == true)
				{// 草に当たったら
					m_bHitGrass = bGrassHit;
				}
			}
		}
		// 次に進める
		pScene = pSceneNext;
	}
}
//=============================================================================
// 当たり判定
//=============================================================================
bool CEnemy::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin)
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
// 敵の生成
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			pEnemy->Init(pos);
			pEnemy->SetType(type);	// 種類
		}
	}
	return pEnemy;
}
////=============================================================================
// 敵の総数取得
//=============================================================================
int CEnemy::GetNumEnemy(void)
{
	return m_nNumEnemy;
}
//=============================================================================
// 敵の総数取得
//=============================================================================
int CEnemy::GetCatchEnemy(void)
{
	return m_nGetEnemy;
}
//=============================================================================
// 敵の種類設定
//=============================================================================
void CEnemy::SetType(ENEMYTYPE type)
{
	m_type = type;
}
