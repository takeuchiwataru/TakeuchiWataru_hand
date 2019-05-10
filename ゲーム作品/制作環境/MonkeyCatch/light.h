//=============================================================================
//
// ライト処理 [light.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT	(3)
#define LIGHT_DIFFUSE000	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
#define LIGHT_DIFFUSE001	(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f))
#define LIGHT_DIFFUSE002	(D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f))
#define LIGHT_VECDIR000	(D3DXVECTOR3(0.22f, -0.87f, 0.44f))
#define LIGHT_VECDIR001	(D3DXVECTOR3(-0.18f, 0.88f, -0.44f))
#define LIGHT_VECDIR002	(D3DXVECTOR3(0.89f, -0.11f, 0.44f))

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLight
{
public:
	CLight();
	~CLight();

	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];			//ライトの情報
};

#endif
