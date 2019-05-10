//=============================================================================
//
// リザルト処理 [result.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CRemain;
class CScore;
class CTimer;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CResult
{
public:
	CResult();
	~CResult();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static CRemain *m_pRemain;
	static CTimer *m_pTimer;
	static CScore *m_pScore;
	static CScore *m_pScoreTime;
	static CScore *m_pScoreTotal;
	static CMeshField *m_pMeshField;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	
	int m_nCounter;		// 描画するタイミング
	int m_nTime;		// 遷移するタイミング
};

#endif