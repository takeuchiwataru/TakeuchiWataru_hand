//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "fade.h"

#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "player.h"
#include "meshfield.h"
#include "meshorbit.h"
#include "object.h"
#include "enemy.h"
#include "sky.h"
#include "meshwall.h"
#include "particle.h"
#include "grass.h"

#include "number.h"
#include "score.h"
#include "frame.h"
#include "remain.h"
#include "image.h"
#include "timer.h"
#include "polygon.h"
#include "pausemat.h"
#include "pauseselect.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CPlayer *CGame::m_pPlayer = NULL;
CScene3D * CGame::m_pScene3D = NULL;
CMeshField * CGame::m_pMeshField = NULL;
CMeshOrbit * CGame::m_pMeshOrbit = NULL;
CScore * CGame::m_pScore = NULL;
CRemain * CGame::m_pRemain = NULL;
CCamera * CGame::m_pCamera = NULL;
CLight * CGame::m_pLight = NULL;
CTimer * CGame::m_pTimer = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_nTime = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
void CGame::Init(void)
{
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

	// ナンバーテクスチャの読み込み
	CNumber::Load();

	// フレームの読み込み
	CFrame::Load();

	// 画像の読み込み
	CImage::Load();

	// パーティクルの読み込み
	CParticle::Load();

	// ポーズ台紙読み込み
	CPauseMat::Load();

	// ポーズ3項目の読み込み
	CPauseSelect::Load();

	//======================
	// 生成
	//======================
	if (m_pPlayer == NULL)
	{// プレイヤーの生成
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(750.0f, 0.0f, -750.0f));
	}

	if (m_pMeshField == NULL)
	{// メッシュフィールドの生成	
		m_pMeshField = CMeshField::Create(D3DXVECTOR3(-500.0f, 0.0f, 500.0f));
	}

	 //敵の生成
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, -300.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(1200.0f, 0.0f, -1200.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(1100.0f, 0.0f, -200.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, -800.0f), CEnemy::ENEMYTYPE_000);
	CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, -500.0f), CEnemy::ENEMYTYPE_002);
	CEnemy::Create(D3DXVECTOR3(1000.0f, 0.0f, -1000.0f), CEnemy::ENEMYTYPE_004);
	CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, -150.0f), CEnemy::ENEMYTYPE_003);
	CEnemy::Create(D3DXVECTOR3(1300.0f, 0.0f, -1000.0f), CEnemy::ENEMYTYPE_005);
	CEnemy::Create(D3DXVECTOR3(1300.0f, 0.0f, -600.0f), CEnemy::ENEMYTYPE_004);
	CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, -1250.0f), CEnemy::ENEMYTYPE_005);

	// 草の生成
	CGrass::Create(D3DXVECTOR3(300.0f, 0.0f, -300.0f),CGrass::GRASSTYPE_001);
	CGrass::Create(D3DXVECTOR3(1200.0f, 0.0f, -1200.0f), CGrass::GRASSTYPE_001);
	CGrass::Create(D3DXVECTOR3(300.0f, 0.0f, -800.0f), CGrass::GRASSTYPE_001);
	CGrass::Create(D3DXVECTOR3(1100.0f, 0.0f, -200.0f), CGrass::GRASSTYPE_001);

	// 壊れない草
	CGrass::Create(D3DXVECTOR3(550.0f, 0.0f, -400.0f),CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(850.0f, 0.0f, -300.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(800.0f, 0.0f, -1150.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(1200.0f, 0.0f, -450.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(350.0f, 0.0f, -630.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(650.0f, 0.0f, -730.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(1100.0f, 0.0f, -830.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(650.0f, 0.0f, -1000.0f), CGrass::GRASSTYPE_000);
	CGrass::Create(D3DXVECTOR3(350.0f, 0.0f, -1100.0f), CGrass::GRASSTYPE_000);

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
	CSky::Create(D3DXVECTOR3(750.0f, 500.0f, -750.0f));

	//======================
	// UI
	//======================

	if (m_pScore == NULL)
	{// スコアクラスの生成
		m_pScore = CScore::Create(D3DXVECTOR3(1200, 80, 0));
	}

	// スコアフレームの生成
	CFrame::Create(D3DXVECTOR3(1000, 80, 0), CFrame::FRAMETYPE_SCORE);

	// UI
	CImage::Create(D3DXVECTOR3(820, 640, 0), CImage::IMAGE_NOKORI);
	CImage::Create(D3DXVECTOR3(1010, 640, 0), CImage::IMAGE_REMAIN);
	if (m_pRemain == NULL)
	{// サルの残りの数の生成
		m_pRemain = CRemain::Create(D3DXVECTOR3(1230, 650, 0));
	}

	// タイマーフレームの生成
	CFrame::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -10, 80, 0), CFrame::FRAMETYPE_TIMER);

	if (m_pTimer == NULL)
	{// タイマーの生成
		m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 80, 0));
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//======================
	// モデルの破棄
	//======================

	// オブジェクトの破棄
	CObject::UnloadX();

	//======================
	// テクスチャの破棄
	//======================

	// ナンバーのテクスチャ破棄
	CNumber::Unload();

	// フレームの破棄
	CFrame::Unload();

	// 画像の破棄
	CImage::Unload();

	// パーティクルのテクスチャ破棄
	CParticle::Unload();

	// ポーズ台紙の破棄
	CPauseMat::Unload();

	// ポーズ3項目の破棄
	CPauseSelect::Unload();

	// 全て解放
	CScene::ReleseAll();

	if (m_pPlayer != NULL)
	{// プレイヤーがNULLじゃなかったらNULLにする
		m_pPlayer = NULL;
	}

	if (m_pMeshField != NULL)
	{// メッシュフィールドがNULLじゃなかったらNULLにする
		m_pMeshField = NULL;
	}

	if (m_pScore != NULL)
	{// スコアがNULLじゃなかったらNULLにする
		m_pScore = NULL;
	}
	if (m_pRemain != NULL)
	{// 残機がNULLじゃなかったらNULLにする
		m_pRemain = NULL;
	}
	if (m_pTimer != NULL)
	{// タイマーがNULLじゃなかったらNULLにする
		m_pTimer = NULL;
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
void CGame::Update(void)
{
	m_nTime++;
	// デバッグ表示
	CDebugProc::Print(1, "ゲーム\n");

	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	/*デバッグ用*/
	//if (m_nTime % 60 == 0)
	//{// 一定時間たったら
	//	pFade->Set(CManager::MODE_GAME, pFade->FADE_OUT);
	//}

	if (m_pCamera != NULL)
	{// カメラの更新処理
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ライトの更新処理
		m_pLight->Update();
	}

#ifdef _DEBUG
	// ワイヤーフレーム
	if (pInputKeyboard->GetPress(DIK_F1) == true)
	{// F1を押したらON
		pDevice->SetRenderState(D3DRS_FILLMODE, 2);
	}
	else if (pInputKeyboard->GetPress(DIK_F2) == true)
	{// F2を押したらOFF
		pDevice->SetRenderState(D3DRS_FILLMODE, 0);
	}
	//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{// フェード
	//	pFade->Set(CManager::MODE_RESULT, pFade->FADE_OUT);
	//}

#endif

}
//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	if (m_pCamera != NULL)
	{// カメラを設置
		m_pCamera->SetCamera();
	}
}
//=============================================================================
// Scene3Dの取得
//=============================================================================
CScene3D *CGame::GetScene3D(void)
{
	return m_pScene3D;
}
//=============================================================================
// メッシュフィールドの取得
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}
//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}
//=============================================================================
// 残機の取得
//=============================================================================
CRemain *CGame::GetRemain(void)
{
	return m_pRemain;
}
//=============================================================================
// スコアの取得
//=============================================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}
//=============================================================================
// カメラの取得
//=============================================================================
CCamera *CGame::GetCamera(void)
{
	return m_pCamera;
}
//=============================================================================
// タイマーの取得
//=============================================================================
CTimer *CGame::GetTimer(void)
{
	return m_pTimer;
}




