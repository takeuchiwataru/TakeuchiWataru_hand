//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CPlayer;
class CMeshField;
class CObject;
class CSky;
class CScore;
class CRemain;
class CTimer;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTutorial
{
public:
	CTutorial();
	~CTutorial();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CMeshField * GetMeshField(void);
	static CPlayer * GetPlayer(void);
	static CRemain * GetRemain(void);
	static CScore * GetScore(void);
	static CCamera * GetCamera(void);


private:
	static CPlayer *m_pPlayer;
	static CMeshField *m_pMeshField;
	static CScore *m_pScore;
	static CRemain *m_pRemain;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CTimer *m_pTimer;
};

#endif