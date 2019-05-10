//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "player.h"
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"

#include "scene3D.h"
#include "meshfield.h"
#include "meshorbit.h"
#include "object.h"
#include "enemy.h"
#include "particle.h"
#include "grass.h"

#include "game.h"
#include "tutorial.h"
#include "scene.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
LPD3DXMESH CPlayer::m_apMesh[MAX_PLAYER_MODEL] = {};
LPD3DXBUFFER CPlayer::m_apBuffMat[MAX_PLAYER_MODEL] = {};
DWORD CPlayer::m_anNumMat[MAX_PLAYER_MODEL] = {};
CMeshOrbit * CPlayer::m_pMeshOrbit = NULL;
CObject * CPlayer::m_pObject = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = PLAYERSTATE_NONE;

	m_pKeyInfo = NULL;
	m_nKey = 0;
	m_nCouterMotion = 0;
	m_nNumParts = 0;
	m_StateSave = 0;

	m_nAttackCounter = 0;
	m_nJumpCounter = 0;

	for (int nCnt = 0; nCnt < MAX_PLAYER_MODEL; nCnt++)
	{
		m_apModel[nCnt] = NULL;
		m_aIndexParent[nCnt] = 0;
		m_aPosOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_aPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aRot[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// モーションの読み込み
	LoadMotion();

	// オブジェクトのタイプの設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	// 情報の初期化
	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = PLAYERSTATE_NONE;
	m_bJump = false;
	m_bLand = false;
	m_fDest = 0.0f;
	m_fDiff = 0.0f;
	m_bAttack = false;
	m_bHit = false;
	m_nAttackCounter = 0;
	m_nJumpCounter = 0;

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
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	if (m_pMeshOrbit != NULL)
	{// メッシュオービットがNULLじゃなかったらNULLにする
		m_pMeshOrbit = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	if (mode == CManager::MODE_TUTORIAL)
	{// モードがチュートリアル
		UpdateTutorial();
	}

	if (mode == CManager::MODE_GAME)
	{// モードがゲーム
		// カメラの取得
		CCamera *pCamera = CGame::GetCamera();
		D3DXVECTOR3 rot = pCamera->GetCamera();

		// キーボードの取得
		CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

		// ジョイパッドの取得
		CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

		// 移動処理
		Move();

		m_pos.y -= m_move.y;
		m_move.y += PLAYER_GRAVITY;		// 重力加算

		// メッシュフィールドの取得
		CMeshField *pMeshField = CGame::GetMeshField();
		float fHight = pMeshField->GetHeight(m_pos);

		if (m_pos.y < fHight)
		{
			m_pos.y = fHight;
			m_bJump = false;
			m_bLand = false;

			if (pInputKeyboard->GetTrigger(DIK_J) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true)
			{// キーボードのJ or Aボタンでジャンプ
				m_bJump = true;
				m_move.y = -PLAYER_JUMP;
				m_nKey = 0;
				m_nCouterMotion = 0;
				pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);
			}
		}
		if (m_bAttack == false)
		{// 攻撃中じゃなかったら
			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
			{// ENTER or Bボタンで虫網
				m_bAttack = true;
				m_nKey = 0;
				m_nCouterMotion = 0;
				pSound->PlaySound(CSound::SOUND_LABEL_SE_MUSIAMI);

				if (m_pMeshOrbit == NULL)
				{// メッシュオービットの生成
					m_pMeshOrbit = CMeshOrbit::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
				}
				// 武器にメッシュオービットを設定
				m_pMeshOrbit->SetMtxParent(&m_apModel[10]->GetMtxWorld());
			}
		}

		if (m_move.x < PLAYER_MOVESTAND && m_move.x > -PLAYER_MOVESTAND && m_move.z < PLAYER_MOVESTAND && m_move.z > -PLAYER_MOVESTAND && m_bJump == false)
		{// ニュートラル
			if (m_state == PLAYERSTATE_MOVE)
			{// 移動してる時
				m_nKey = 0;
				m_nCouterMotion = 0;
			}
			m_state = PLAYERSTATE_NORMAL;
		}
		if (m_move.x >= PLAYER_MOVESTAND || m_move.x <= -PLAYER_MOVESTAND || m_move.z >= PLAYER_MOVESTAND || m_move.z <= -PLAYER_MOVESTAND && m_bJump == false)
		{// 移動
			m_state = PLAYERSTATE_MOVE;

			for (int nCntParticle = 0; nCntParticle < P_MAX_PARTICLE; nCntParticle++)
			{// パーティクルの生成
				CParticle::Create(P_PARTICLE_CREATEPOS, D3DXVECTOR3(sinf(P_PARTICLE_AROUND) * P_PARTICLE_RAND, cosf(P_PARTICLE_AROUND) * P_PARTICLE_RAND, 0.0f),
					P_PARTICLE_LIFE, P_PARTICLE_RADIUS, CParticle::PARTICLETYPE_001);
			}
		}
		if (m_bJump == true)
		{// ジャンプ
			m_state = PLAYERSTATE_JUMP;
		}
		if (m_bLand == true)
		{// 着地
			m_state = PLAYERSTATE_LANDING;
		}
		if (m_bAttack == true)
		{// 虫網振る
			m_nAttackCounter++;
			m_state = PLAYERSTATE_MUSIAMI;

			if (m_nAttackCounter >= ATTACK_COUNTER)
			{// 一定時間たったら
				m_bAttack = false;
				m_nAttackCounter = 0;
			}
		}

		// 位置更新
		m_pos.x += m_move.x;
		m_pos.z += m_move.z;

		// 減速
		m_move.x += (0.0f - m_move.x) * 0.7f;
		m_move.z += (0.0f - m_move.z) * 0.7f;

		// モーションの更新
		UpdateMotion();

		// 当たり判定全般
		CollisionAll();

		// マップ当たり判定
		MapCollision();
	}

	// デバッグ表示
	CDebugProc::Print(1, "プレイヤーの操作 : [↑,↓,←,→  移動][ENTER 虫網][J ジャンプ]\n");
	CDebugProc::Print(1, "プレイヤーの位置 : X %.1f, Y %.1f, Z %.1f\n", m_pos.x, m_pos.y, m_pos.z);
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::UpdateTutorial(void)
{
	// カメラの取得
	CCamera *pCamera = CTutorial::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetCamera();

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// 過去の位置を取得
	m_posold = m_pos;

	//===================
	// キーボード
	//===================
	if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_A) == true)
	{// ← or Aを押したら
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//左上移動
			m_move.x -= sinf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//左下移動
			m_move.x -= sinf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.25f);
		}
		else
		{// 左移動
			m_move.x -= sinf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{// → or Dを押したら
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//右上移動
			m_move.x -= sinf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//右下移動
			m_move.x -= sinf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.25f);
		}
		else
		{// 右移動
			m_move.x -= sinf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
	{// ↑ or Wを押したら
		m_move.x += sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z += cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 1.0f);
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
	{// ↓ or Sを押したら
		m_move.x -= sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z -= cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 0.0f);
	}

	//===================
	// コントローラー
	//===================
	if (pInputJoyPad != NULL)
	{
		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_20) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_21) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_22) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_23) == true)
		{// 左スティック
			m_move.x -= sinf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;

			m_fDest = rot.y - pInputJoyPad->GetLeftAxiz();
		}

		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_28) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_29) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_30) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_31) == true)
		{// 十字キー
			m_move.x += sinf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;
			m_move.z += cosf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;

			m_fDest = rot.y + pInputJoyPad->GetRadian() - D3DX_PI;
		}
	}

	// 角度の設定
	SetAngle();

	m_pos.y -= m_move.y;
	m_move.y += PLAYER_GRAVITY;		// 重力加算

	// メッシュフィールドの取得
	CMeshField *pMeshField = CTutorial::GetMeshField();
	float fHight = pMeshField->GetHeight(m_pos);

	if (m_pos.y < fHight)
	{
		m_pos.y = fHight;
		m_bJump = false;
		m_bLand = false;

		if (pInputKeyboard->GetTrigger(DIK_J) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true)
		{// キーボードのJ or Aボタンでジャンプ
			m_bJump = true;
			m_move.y = -PLAYER_JUMP;
			m_nKey = 0;
			m_nCouterMotion = 0;
			pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);
		}
	}
	if (m_bAttack == false)
	{// 攻撃中じゃなかったら
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
		{// ENTER or Bボタンで虫網
			m_bAttack = true;
			m_nKey = 0;
			m_nCouterMotion = 0;
			pSound->PlaySound(CSound::SOUND_LABEL_SE_MUSIAMI);

			if (m_pMeshOrbit == NULL)
			{// メッシュオービットの生成
				m_pMeshOrbit = CMeshOrbit::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
			}
			// 武器にメッシュオービットを設定
			m_pMeshOrbit->SetMtxParent(&m_apModel[10]->GetMtxWorld());
		}
	}

	if (m_move.x < PLAYER_MOVESTAND && m_move.x > -PLAYER_MOVESTAND && m_move.z < PLAYER_MOVESTAND && m_move.z > -PLAYER_MOVESTAND && m_bJump == false)
	{// ニュートラル
		if (m_state == PLAYERSTATE_MOVE)
		{// 移動してる時
			m_nKey = 0;
			m_nCouterMotion = 0;
		}

		m_state = PLAYERSTATE_NORMAL;
	}
	if (m_move.x >= PLAYER_MOVESTAND || m_move.x <= -PLAYER_MOVESTAND || m_move.z >= PLAYER_MOVESTAND || m_move.z <= -PLAYER_MOVESTAND && m_bJump == false)
	{// 移動
		m_state = PLAYERSTATE_MOVE;
	}
	if (m_bJump == true)
	{// ジャンプ
		m_state = PLAYERSTATE_JUMP;
	}
	if (m_bLand == true)
	{// 着地
		m_state = PLAYERSTATE_LANDING;
	}
	if (m_bAttack == true)
	{// 虫網振る
		m_nAttackCounter++;
		m_state = PLAYERSTATE_MUSIAMI;

		if (m_nAttackCounter >= ATTACK_COUNTER)
		{// 一定時間たったら
			m_bAttack = false;
			m_nAttackCounter = 0;
		}
	}

	// 位置更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	// 減速
	m_move.x += (0.0f - m_move.x) * 0.7f;
	m_move.z += (0.0f - m_move.z) * 0.7f;

	// モーションの更新
	UpdateMotion();

	// 当たり判定全般
	CollisionAll();

	// マップ当たり判定
	MapCollision();
}
//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
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

	vecLight = PLAYER_VECLIGHT;
	pos = PLAYER_POS;
	normal = PLAYER_NORMAL;

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
//============================================================================
// 移動処理
//=============================================================================
void CPlayer::Move(void)
{
	// カメラの取得
	CCamera *pCamera = CGame::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetCamera();

	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// 過去の位置を取得
	m_posold = m_pos;

	//===================
	// キーボード
	//===================
	if (pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_A) == true)
	{// ← or Aを押したら
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//左上移動
			m_move.x -= sinf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//左下移動
			m_move.x -= sinf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.25f);
		}
		else
		{// 左移動
			m_move.x -= sinf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y + D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y + D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{// → or Dを押したら
		if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
		{//右上移動
			m_move.x -= sinf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.75f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.75f);
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
		{//右下移動
			m_move.x -= sinf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.25f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.25f);
		}
		else
		{// 右移動
			m_move.x -= sinf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - D3DX_PI * 0.5f) * PLAYER_MOVE_SPEED;
			m_fDest = (rot.y - D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_W) == true)
	{// ↑ or Wを押したら
		m_move.x += sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z += cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 1.0f);
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_S) == true)
	{// ↓ or Sを押したら
		m_move.x -= sinf(rot.y) * PLAYER_MOVE_SPEED;
		m_move.z -= cosf(rot.y) * PLAYER_MOVE_SPEED;
		m_fDest = (rot.y + D3DX_PI * 0.0f);
	}

	//===================
	// コントローラー
	//===================
	if (pInputJoyPad != NULL)
	{
		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_20) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_21) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_22) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_23) == true)
		{// 左スティック
			m_move.x -= sinf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;
			m_move.z -= cosf(rot.y - pInputJoyPad->GetLeftAxiz()) * PLAYER_MOVE_SPEED;

			m_fDest = rot.y - pInputJoyPad->GetLeftAxiz();
		}

		if (pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_28) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_29) == true
			|| pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_30) == true || pInputJoyPad->GetPress(CInputJoyPad::DIJS_BUTTON_31) == true)
		{// 十字キー
			m_move.x += sinf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;
			m_move.z += cosf(rot.y + pInputJoyPad->GetRadian())* PLAYER_MOVE_SPEED;

			m_fDest = rot.y + pInputJoyPad->GetRadian() - D3DX_PI;
		}
	}

	// 角度の設定
	SetAngle();
}
//=============================================================================
// モーションテキストの読み込み
//=============================================================================
void CPlayer::LoadMotion(void)
{
	FILE * pFile;
	char *pStrCur;						// 文字列の先頭ポインタ
	char aLine[MAX_CHAR];					// 文字列読み込み用
	char aStr[MAX_CHAR];						// 文字列抜き出し用
	int nWord;
	int nSpace;
	int nIndex;							// インデックス
	int nLoop;							// ループ
	int nCntMotion = 0;					// モーション

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ファイルオープン
	pFile = fopen(PLAYER_FILE, "r");

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

				strcpy(&aStr[0], pStrCur);		// 対象の文字列抜き出し

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

							m_aPos[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							m_aPos[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							m_aPos[nIndex].z = (float)atof(pStrCur);	// Z

							// 1行読み込み
							pStrCur = ReadLine(pFile, &aLine[0]);
							pStrCur = GetLineTop(&aLine[0]);

							strcpy(&aStr[0], pStrCur);
						}

						if (memcmp(&aStr[0], ROT, strlen(ROT)) == 0)
						{// 向き
							pStrCur += strlen(ROT);		// 頭出し

							m_aRot[nIndex].x = (float)atof(pStrCur);	// X

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							m_aRot[nIndex].y = (float)atof(pStrCur);	// Y

							nWord = PopString(pStrCur, &aStr[0]);

							pStrCur += nWord;	// 次まで進める

							m_aRot[nIndex].z = (float)atof(pStrCur);	// Z

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
							m_apModel[0] = CModel::Create(D3DXVECTOR3(m_pos.x + m_aPos[nIndex].x, m_pos.y + m_aPos[nIndex].y, m_aPos[nIndex].z));
							m_apModel[0]->BindModel(m_apMesh[nIndex], m_apBuffMat[nIndex], m_anNumMat[nIndex]);
							m_apModel[0]->SetParent(NULL);			// 体

							// オフセットに代入
							m_aPosOffset[0] = m_apModel[0]->GetPos();
						}
						else if (m_aIndexParent[nIndex] > -1)
						{// 子
							m_apModel[nIndex] = CModel::Create(D3DXVECTOR3(m_aPos[nIndex].x, m_aPos[nIndex].y, m_aPos[nIndex].z));
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
void CPlayer::UpdateMotion(void)
{
	m_StateSave = m_state - 1;

	P_KEY *pKey, *pKeyNext;				// 現在のキー,次のキー
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
				m_nKey += 1;
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

	// デバッグ表示
	CDebugProc::Print(1, " %d / %d\n", m_nCouterMotion, m_aMotionInfo[m_StateSave].aKeyInfo[m_nKey].nFrame);
	CDebugProc::Print(1, "現在のキー :  %d\n", m_nKey);
	CDebugProc::Print(1, "プレイヤーの状態 : %d\n\n", m_state);
}
//=============================================================================
// 無効な行無視
//=============================================================================
char *CPlayer::ReadLine(FILE * pFile, char *pDst)
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
char *CPlayer::GetLineTop(char *pSrc)
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
int CPlayer::PopString(char * pSrc, char * pDst)
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
// プレイヤーの生成
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			pPlayer->Init(pos);
		}
	}
	return pPlayer;
}
//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// 移動取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}
//=============================================================================
// 状態取得
//=============================================================================
CPlayer::PLAYERSTATE CPlayer::GetState(void)
{
	return m_state;
}
//=============================================================================
// 角度の設定
//=============================================================================
void CPlayer::SetAngle(void)
{
	m_fDiff = m_fDest - m_rot.y;

	if (m_fDiff > D3DX_PI)
	{
		m_fDiff -= D3DX_PI* PLAYER_AROUND;
	}
	if (m_fDiff < -D3DX_PI)
	{
		m_fDiff += D3DX_PI* PLAYER_AROUND;
	}

	m_rot.y += m_fDiff * PLAYER_DIFF;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI* PLAYER_AROUND;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI* PLAYER_AROUND;
	}
}
//=============================================================================
// マップ当たり判定
//=============================================================================
void CPlayer::MapCollision(void)
{
	if (m_pos.z > -PLAYER_MIN_POS)
	{// Zの奧
		m_pos.z = -PLAYER_MIN_POS;
	}
	else if (m_pos.z < -PLAYER_MAX_POS)
	{// Zの手前
		m_pos.z = -PLAYER_MAX_POS;
	}
	else if (m_pos.x < PLAYER_MIN_POS)
	{// Xの左
		m_pos.x = PLAYER_MIN_POS;
	}
	else if (m_pos.x > PLAYER_MAX_POS)
	{// Xの右
		m_pos.x = PLAYER_MAX_POS;
	}
}
//=============================================================================
// 当たり判定全般
//=============================================================================
void CPlayer::CollisionAll(void)
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
				((CObject*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
			else if (objType == CScene::OBJTYPE_ENEMY)
			{// 敵だったら
				((CEnemy*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
			else if (objType == CScene::OBJTYPE_GRASS)
			{// 草だったら
				((CGrass*)pScene)->Collision(&m_pos, &m_posold, &m_move, &m_VtxMax, &m_VtxMin);
			}
		}
		// 次に進める
		pScene = pSceneNext;
	}
}
//=============================================================================
// 当たり判定
//=============================================================================
bool CPlayer::AttackCollision(D3DXVECTOR3 pos, float radius)
{
	m_bHit = false;	// 当たったかどうか

	if (m_bAttack == true)
	{// プレイヤーが攻撃状態なら当たり判定ON

		// 武器のワールドマトリックス取得
		float fXmtx = pos.x - (m_apModel[10]->GetMtxWorld()._41);
		float fYmtx = pos.y + 30 - (m_apModel[10]->GetMtxWorld()._42);
		float fZmtx = pos.z - (m_apModel[10]->GetMtxWorld()._43);

		// 合計の計算
		float fTotal = (float)pow(fXmtx, 2) + (float)pow(fYmtx, 2) + (float)pow(fZmtx, 2);	// pow(x,y）xのy乗で計算される

		if (fTotal <= radius)
		{// 武器と敵の判定
			m_bHit = true;
			//CDebugProc::Print(1, "当たった\n");
		}
	}
	return m_bHit;
}

