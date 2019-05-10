//=============================================================================
//
// ポーズ台紙処理 [pausemat.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _PAUSEMAT_H_
#define _PAUSEMAT_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSEMAT_WIDTH (250)
#define PAUSEMAT_HEIGHT (250)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPauseMat : public CLogo
{
public:
	CPauseMat();
	~CPauseMat();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPauseMat * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャのポインタ
};
#endif
