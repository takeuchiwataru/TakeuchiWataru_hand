//=============================================================================
//
// タイトル処理 [title.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTitle
{
public:
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nTime;		// 遷移するタイミング
	static CMeshField *m_pMeshField;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
};

#endif