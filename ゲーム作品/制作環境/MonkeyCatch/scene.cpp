//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "debugproc.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CScene * CScene::m_apTop[NUM_PRIORITY] = {};
CScene * CScene::m_apCur[NUM_PRIORITY] = {};

int CScene::m_nNumAll = 0;
int CScene::m_nNumPriority[NUM_PRIORITY] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	// 死亡フラグOFF
	m_bDeath = false;

	if (m_apCur[nPriority] != NULL)
	{// カレントがNULLじゃなかったら
		m_apCur[nPriority]->m_pNext = this;
	}
	else
	{
		m_apTop[nPriority] = this;
	}

	m_pPrev = m_apCur[nPriority];
	m_pNext = NULL;

	m_apCur[nPriority] = this;

	m_nPriority = nPriority;		// 優先順位の設定
	m_objType = objType;			// オブジェクトの種類設定
	m_nNumPriority[nPriority]++;
}
//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}
//=============================================================================
// 全て解放
//=============================================================================
void CScene::ReleseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		if (m_apTop[nCntPriority] != NULL)
		{
			while (m_apTop[nCntPriority])
			{
				// 終了処理
				m_apTop[nCntPriority]->Uninit();

				// 全て終了
				m_apTop[nCntPriority]->UninitAll(nCntPriority);
			}
		}
	}
}
//=============================================================================
// 全て更新
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CScene * pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene)
		{
			CScene * pSceneNext = pScene->m_pNext;	// 遺言  /* 更新したときに消える可能性があるから */ 

			// 更新処理
			pScene->Update();

			pScene = pSceneNext;
		}
		//CDebugProc::Print(1, "%d番目 : %d\n", nCntPriority, m_nNumPriority[nCntPriority]);
	}
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CScene * pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene)
		{
			CScene * pSceneNext = pScene->m_pNext;	// 遺言  /* 更新したときに消える可能性があるから */ 

			if (pScene->m_bDeath == true)
			{// 死亡フラグが立っていれば
				pScene->UninitAll(nCntPriority);
			}

			pScene = pSceneNext;
		}
		//CDebugProc::Print(1, "%d番目 : %d\n", nCntPriority, m_nNumPriority[nCntPriority]);
	}
	//CDebugProc::Print(1, "\n");
}
//=============================================================================
// 全て描画
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CScene * pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene)
		{
			CScene * pSceneNext = pScene->m_pNext;	// 遺言

			// 描画処理
			pScene->Draw();

			pScene = pSceneNext;
		}
	}
}
//=============================================================================
// 全て終了
//=============================================================================
void CScene::UninitAll(int nPriority)
{
	if (m_pPrev != NULL)
	{// 前のオブジェクトがNULLじゃなかったら
		m_pPrev->m_pNext = m_pNext;
	}
	else
	{
		m_apTop[nPriority] = m_pNext;
	}
	if (m_pNext != NULL)
	{//　次のオブジェクトがNULLじゃなかったら
		m_pNext->m_pPrev = m_pPrev;
	}
	else
	{
		m_apCur[nPriority] = m_pPrev;
	}
	delete this;

	m_nNumPriority[nPriority]--;
}

//=============================================================================
// オブジェクトの破棄(死亡フラグ)
//=============================================================================
void CScene::Release(void)
{
	// 死亡フラグON
	m_bDeath = true;
}
//=============================================================================
// オブジェクトの種類設定
//=============================================================================
void CScene::SetObjType(OBJTYPE ObjType)
{
	m_objType = ObjType;
}
//=============================================================================
// オブジェクトの種類取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}
//=============================================================================
// オブジェクトの先頭取得
//=============================================================================
CScene *CScene::GetTop(int nPriority)
{
	return m_apTop[nPriority];
}
//=============================================================================
// オブジェクトの次の取得
//=============================================================================
CScene *CScene::GetNext(void)
{
	return m_pNext;
}
//=============================================================================
// 死亡フラグの取得
//=============================================================================
bool CScene::GetDeath(void)
{
	return m_bDeath;
}
