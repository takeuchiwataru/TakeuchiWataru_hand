//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CTitle;
class CGame;
class CResult;
class CRanking;
class CTutorial;
class CFade;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING (5)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CManager
{
public:
	typedef enum
	{// モードの種類
		MODE_TITLE = 0,		// タイトル
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_RANKING,		// ランキング
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetMode(MODE mode);
	static MODE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputJoyPad *GetJoyPad(void);
	static CSound *GetSound(void);
	static CFade *GetFade(void);
	static int GetRanking(int index);
	static void GetRankingScore(int nScore);

private:
	static CRenderer *m_pRenderer;
	static CDebugProc *m_pDebugProc;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputJoyPad *m_pInputJoyPad;
	//static CCamera *m_pCamera;
	//static CLight *m_pLight;
	static CSound *m_pSound;
	static MODE m_mode;

	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CRanking *m_pRanking;
	static CTutorial *m_pTutorial;
	static CFade *m_pFade;
	static int m_aScore[MAX_RANKING];


	//static CScene3D * m_pScene3D;
	//static CMeshField * m_pMeshField;
	//static CMeshOrbit * m_pMeshOrbit;
};
#endif