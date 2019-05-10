//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "tutorial.h"
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
#include "logo.h"
#include "image.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CPlayer *CTutorial::m_pPlayer = NULL;
CMeshField * CTutorial::m_pMeshField = NULL;
CScore * CTutorial::m_pScore = NULL;
CRemain * CTutorial::m_pRemain = NULL;
CCamera * CTutorial::m_pCamera = NULL;
CLight * CTutorial::m_pLight = NULL;
CTimer * CTutorial::m_pTimer = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
void CTutorial::Init(void)
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

	// チュートリアルロゴの読み込み
	CTutorialLogo::Load();

	// プレスエンターの読み込み
	CPressEnter::Load();

	// 画像の読み込み
	CImage::Load();

	// ナンバーテクスチャの読み込み
	CNumber::Load();

	// フレームの読み込み
	CFrame::Load();

	// パーティクルの読み込み
	CParticle::Load();

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

	// 敵の生成
	CEnemy::Create(D3DXVECTOR3(550.0f, 0.0f, -750.0f), CEnemy::ENEMYTYPE_000);

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

	// チュートリアルロゴの生成
	CTutorialLogo::Create(D3DXVECTOR3(150, 50, 0.0f));

	// プレスエンタークラスの生成
	CPressEnter::Create(D3DXVECTOR3(1120, 700, 0.0f), CPressEnter::PRESSENTER_TUTORIAL);
	CPressEnter::DispPressEnter();

	// コントローラーの説明
	CImage::Create(D3DXVECTOR3(200, 550, 0), CImage::IMAGE_TUTORIAL);

	// タイマーフレームの生成
	CFrame::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 10, 80, 0), CFrame::FRAMETYPE_TIMER);

	if (m_pTimer == NULL)
	{// タイマーの生成
		m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 80, 0));
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
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

	// パーティクルのテクスチャ破棄
	CParticle::Unload();

	// チュートリアルロゴテクスチャ破棄
	CTutorialLogo::Unload();

	// プレスエンターテクスチャ破棄
	CPressEnter::Unload();

	// 画像の破棄
	CImage::Unload();

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
void CTutorial::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	if (pInputJoyPad != NULL)
	{
		if (pInputKeyboard->GetTrigger(DIK_1) == true||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_12) == true)
		{// 対象のボタンが押されたら(1,STARTボタン)
			pFade->Set(CManager::MODE_GAME, pFade->FADE_OUT);
		}
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
	//	CManager::SetMode(CManager::MODE_GAME);
	//}

	// デバッグ表示
	CDebugProc::Print(1, "チュートリアル\n");
}
//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	if (m_pCamera != NULL)
	{// カメラを設置
		m_pCamera->SetCamera();
	}
}
//=============================================================================
// メッシュフィールドの取得
//=============================================================================
CMeshField *CTutorial::GetMeshField(void)
{
	return m_pMeshField;
}
//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}
//=============================================================================
// 残機の取得
//=============================================================================
CRemain *CTutorial::GetRemain(void)
{
	return m_pRemain;
}
//=============================================================================
// スコアの取得
//=============================================================================
CScore *CTutorial::GetScore(void)
{
	return m_pScore;
}
//=============================================================================
// カメラの取得
//=============================================================================
CCamera *CTutorial::GetCamera(void)
{
	return m_pCamera;
}
