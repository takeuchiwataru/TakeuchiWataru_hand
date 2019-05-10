//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "logo.h"
#include "number.h"
#include "remain.h"
#include "enemy.h"
#include "image.h"
#include "timer.h"
#include "score.h"

#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "player.h"
#include "meshfield.h"
#include "object.h"
#include "enemy.h"
#include "sky.h"
#include "particle.h"
#include "grass.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRemain * CResult::m_pRemain = NULL;
CTimer * CResult::m_pTimer = NULL;
CScore * CResult::m_pScore = NULL;
CScore * CResult::m_pScoreTime = NULL;
CScore * CResult::m_pScoreTotal = NULL;
CMeshField * CResult::m_pMeshField = NULL;
CCamera * CResult::m_pCamera = NULL;
CLight * CResult::m_pLight = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
	m_nCounter = 0;
	m_nTime = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
void CResult::Init(void)
{
	m_nCounter = 0;
	m_nTime = 0;

	// カメラクラスの生成
	if (m_pCamera == NULL)
	{// 空
		m_pCamera = new CCamera;

		m_pCamera->Init();
	}

	// ライトクラスの生成
	if (m_pLight == NULL)
	{// 空
		m_pLight = new CLight;

		m_pLight->Init();
	}

	//======================
	// モデルの読み込み
	//======================

	// オブジェクトの読み込み
	CObject::LoadX();

	//======================
	// テクスチャの読み込み
	//======================

	// リザルトロゴの読み込み
	CResultLogo::Load();

	// ナンバーテクスチャの読み込み
	CNumber::Load();

	// UI画像の読み込み
	CImage::Load();


	//======================
	// 生成
	//======================

	if (m_pMeshField == NULL)
	{// メッシュフィールドの生成	
		m_pMeshField = CMeshField::Create(D3DXVECTOR3(-500.0f, 0.0f, 500.0f));
	}

	//==============
	// Xの左側
	//==============
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -620.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -820.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -1020.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -1220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(20.0f, 0.0f, -1420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	//==============
	// Xの右側
	//==============
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -620.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -820.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -1020.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -1220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1480.0f, 0.0f, -1420.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	//==============
	// Zの奧
	//==============
	CObject::Create(D3DXVECTOR3(150.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(350.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(550.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(750.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(950.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1150.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1350.0f, 0.0f, -20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	//==============
	// Zの手前
	//==============
	CObject::Create(D3DXVECTOR3(150.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(350.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(550.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(750.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(950.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1150.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);
	CObject::Create(D3DXVECTOR3(1350.0f, 0.0f, -1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject::OBJECTTYPE_TREE);

	// 空の生成
	CSky::Create(D3DXVECTOR3(750.0f, 480.0f, -750.0f));

	// リザルトロゴクラスの生成
	CResultLogo::Create(D3DXVECTOR3(150, 50, 0.0f),CResultLogo::RESULTTYPE_CLEAR);
}
//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//======================
	// モデルの破棄
	//======================

	// オブジェクトの破棄
	CObject::UnloadX();

	//======================
	// テクスチャの破棄
	//======================

	// リザルトロゴの破棄
	CResultLogo::Unload();

	// ナンバーのテクスチャ破棄
	CNumber::Unload();

	// UI画像のテクスチャ破棄
	CImage::Unload();

	// 全て解放
	CScene::ReleseAll();

	if (m_pRemain != NULL)
	{// 残機がNULLじゃなかったらNULLにする
		m_pRemain = NULL;
	}
	if (m_pTimer != NULL)
	{// タイマーがNULLじゃなかったらNULLにする
		m_pTimer = NULL;
	}
	if (m_pScore != NULL)
	{// スコア(敵)がNULLじゃなかったらNULLにする
		m_pScore = NULL;
	}
	if (m_pScoreTime != NULL)
	{// スコア(タイム)がNULLじゃなかったらNULLにする
		m_pScoreTime = NULL;
	}
	if (m_pScoreTotal != NULL)
	{// 合計スコアがNULLじゃなかったらNULLにする
		m_pScoreTotal = NULL;
	}
	
	if (m_pMeshField != NULL)
	{// メッシュフィールドがNULLじゃなかったらNULLにする
		m_pMeshField = NULL;
	}

	//　カメラの破棄
	if (m_pCamera != NULL)
	{
		// 終了処理
		m_pCamera->Uninit();

		// データの開放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//　ライトの破棄
	if (m_pLight != NULL)
	{
		// 終了処理
		m_pLight->Uninit();

		// データの開放
		delete m_pLight;
		m_pLight = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	m_nCounter++;
	m_nTime++;

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	// ゲットした敵の数取得
	int nGetEnemy = CEnemy::GetCatchEnemy();

	// 止めた時間の取得
	int nTime = CTimer::GetStopTimer();

	if (m_nCounter == 60)
	{// 1秒後
		CImage::Create(D3DXVECTOR3(300, 200, 0), CImage::IMAGE_CATCHNUM);	// 捕まえた数

		if (m_pRemain == NULL)
		{// サルの捕まえた数の生成

			m_pRemain = CRemain::Create(D3DXVECTOR3(700, 200, 0));
			m_pRemain->AddRemain(nGetEnemy);
			m_pRemain->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (m_pScore == NULL)
		{// 敵スコアの生成
			m_pScore = CScore::Create(D3DXVECTOR3(1200, 200, 0));
			m_pScore->AddScore(nGetEnemy * 1000);
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JYAN);
	}
	if (m_nCounter == 120)
	{// 2秒後
		CImage::Create(D3DXVECTOR3(335, 400, 0), CImage::IMAGE_REMAINTIME);	// 残り時間

		if (m_pTimer == NULL)
		{// 残り時間の生成
			m_pTimer = CTimer::Create(D3DXVECTOR3(700, 400, 0));
			m_pTimer->AddTime(nTime);
		}
		if (m_pScoreTime == NULL)
		{// タイムスコアの生成
			m_pScoreTime = CScore::Create(D3DXVECTOR3(1200, 400, 0));
			m_pScoreTime->AddScore(nTime * 10);
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JYAN);
	}
	if (m_nCounter == 180)
	{// 3秒後
		CImage::Create(D3DXVECTOR3(410, 600, 0), CImage::IMAGE_TOTAL);		// 合計

		if (m_pScoreTotal == NULL)
		{// 合計スコア
			m_pScoreTotal = CScore::CreateTotal(D3DXVECTOR3(1200, 600, 0));
			m_pScoreTotal->AddScore(nGetEnemy * 1000 + nTime * 10);
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JYAN);
	}
		 
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	if (m_nTime % 300 == 0)
	{// 一定時間たったら
		pFade->Set(CManager::MODE_RANKING, pFade->FADE_OUT);
		CManager::GetRankingScore(m_pScoreTotal->GetScore());
	}

	if (m_pCamera != NULL)
	{// カメラの更新処理
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ライトの更新処理
		m_pLight->Update();
	}

	///*デバッグ用*/
	//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{
	//	CManager::SetMode(CManager::MODE_RANKING);
	//}

	// デバッグ表示
	CDebugProc::Print(1, "リザルト\n");
}
//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	if (m_pCamera != NULL)
	{// カメラを設置
		m_pCamera->SetCamera();
	}
}