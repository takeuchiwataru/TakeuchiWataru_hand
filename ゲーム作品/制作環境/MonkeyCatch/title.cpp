//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "title.h"
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
#include "logo.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CMeshField * CTitle::m_pMeshField = NULL;
CCamera * CTitle::m_pCamera = NULL;
CLight * CTitle::m_pLight = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_nTime = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
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

	// タイトルロゴの読み込み
	CTitleLogo::Load();

	// プレスエンターの読み込み
	CPressEnter::Load();

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
	CSky::Create(D3DXVECTOR3(750.0f, 500.0f, -750.0f));

	//======================
	// UI
	//======================

	// タイトルロゴクラスの生成
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	// プレスエンターの生成
	CPressEnter::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250, 0.0f), CPressEnter::PRESSENTER_TITLE);
	CPressEnter::DispPressEnter();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//======================
	// モデルの破棄
	//======================

	// オブジェクトの破棄
	CObject::UnloadX();

	//======================
	// テクスチャの破棄
	//======================

	// タイトルロゴテクスチャ破棄
	CTitleLogo::Unload();

	// プレスエンターテクスチャ破棄
	CPressEnter::Unload();

	// 全て解放
	CScene::ReleseAll();

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
void CTitle::Update(void)
{
	m_nTime++;

	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	if (pInputJoyPad != NULL)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_1) == true ||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_2) == true ||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true ||
			pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true )
		{// 対象のボタンが押されたら
			pFade->Set(CManager::MODE_TUTORIAL, pFade->FADE_OUT);
		}
	}
	if (m_nTime % 300 == 0)
	{// 一定時間たったら
		pFade->Set(CManager::MODE_RANKING, pFade->FADE_OUT);
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
	//	CManager::SetMode(CManager::MODE_TUTORIAL);
	//}

	// デバッグ表示
	CDebugProc::Print(1, "タイトル\n");
}
//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	if (m_pCamera != NULL)
	{// カメラを設置
		m_pCamera->SetCamera();
	}
}
